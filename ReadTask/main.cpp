#include <iostream>
#include <memory>
//#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <list>
#include <ctime>
#include "Config.h"

#pragma warning(disable : 4996)
//#define CLOCK_PER_MSEC CLOCK_PER_SEC / 1000;
using namespace std;

bool reading_is_finished = false;
uint64_t number_of_all_bytes = 0;
list<string> content;
bool file_read = false;
string error = "";

void read_file_content(Config* config) {
	for (string name : config->file_names) {
		content.push_back("__" + name + "__");

		{
			auto stream = unique_ptr<FILE, decltype(&fclose)>(fopen((config->path + name).c_str(), "rb"), &fclose);
			
			if (stream != NULL) {
				fseek(stream.get(), 0, SEEK_END);
				uint64_t size = ftell(stream.get());
				rewind(stream.get());

				//cout << size << endl;

				clock_t start = clock();
				uint16_t read_bytes = 0;

				while (ftell(stream.get()) < size) {
					if (read_bytes <= config->speed || config->speed == -1) {
						uint32_t bytes;

						fread(&bytes, sizeof(bytes), 1, stream.get());
						content.push_back(to_string(bytes));
						fseek(stream.get(), sizeof(bytes), SEEK_CUR);

						number_of_all_bytes += sizeof(bytes);
						read_bytes += sizeof(bytes);
					}
					else {
						while ((clock() - start) <= CLOCKS_PER_SEC);// cout << "here is a loop";
						//number_of_all_bytes += read_bytes;
						read_bytes = 0;
						start = clock();
					}
				}
				file_read = true;
			}
			else
				error += "[error] File " + config->path + name + " does not exist\n";
		}
	}
	reading_is_finished = true;
}

clock_t start_time = 0, end_time;

void print_time() {
	string dots = "";
	while (!reading_is_finished) {
		clock_t now = clock();
		end_time = now;

		system("cls");

		cout << "Reading continues" + dots << endl;
		dots += ".";
		cout << "Time has passed " << (double) (now - start_time) / CLOCKS_PER_SEC << " seconds" << endl;
		cout << "How many values have already been read " << content.size() << endl;
	}
}

int main() {
	Config config("config.toml");

	if (!config.error) {
		thread content_reading(read_file_content, &config);
		thread duration(print_time);

		start_time = clock();

		content_reading.join();
		duration.join();

		/*for (string str : content) {
			cout << str << endl;
		}*/

		clock_t took_time = end_time - start_time;

		if (!error.empty())
			cout << endl << error << endl;
		if (file_read) {
			cout << "Whole reading took " << (double)took_time / CLOCKS_PER_SEC << " seconds" << endl;
			cout << "Speed of reading " << (double)(number_of_all_bytes * CLOCKS_PER_SEC) / took_time << " bytes per second" << endl;
		}
	}
	system("pause");

	return 0;
}