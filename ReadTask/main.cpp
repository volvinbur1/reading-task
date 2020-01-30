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

using namespace std;

bool reading_is_finished = false;
uint64_t number_of_bits = 0;
list<string> content;

void read_file_content(Config* config) {
	for (string name : config->file_names) {
		content.push_back("__" + name + "__");

		{
			auto stream = unique_ptr<FILE, decltype(&fclose)>(fopen(name.c_str(), "rb"), &fclose);
			
			if (stream != NULL) {
				fseek(stream.get(), 0, SEEK_END);
				uint64_t size = ftell(stream.get());
				rewind(stream.get());

				//cout << size << endl;

				while (ftell(stream.get()) < size) {
					uint32_t bytes;
					fread(&bytes, sizeof(uint32_t), 1, stream.get());
					content.push_back(to_string(bytes));
					fseek(stream.get(), sizeof(uint32_t), SEEK_CUR);
					number_of_bits += sizeof(uint32_t) * 8;
				}
			}
			//unique_ptr<ifstream> input(new ifstream);// = make_unique<ifstream>();

			//input->open(config->path + name);

			//if (input->is_open()) {
			//	string line;
			//	while (getline(*input, line))
			//		content.push_back(line);
			//}
		}
	}
	reading_is_finished = true;
}

clock_t start_time = 0, end_time;

void print_time() {
	while (!reading_is_finished) {
		clock_t now = clock();
		end_time = now;
		string dots = "";

		system("cls");

		cout << "Reading continues" + dots << endl;
		dots += ".";
		cout << "Time has passed " << now - start_time << " seconds" << endl;
	}
}

int main() {
	Config config("config.toml");

	thread content_reading(read_file_content, &config);
	thread duration(print_time);

	start_time = clock();

	content_reading.join();
	duration.join();

	for (string str : content) {
		cout << str << endl;
	}

	clock_t tooked_time = end_time - start_time;

	cout << "Whole reading took " << (double) tooked_time / CLOCKS_PER_SEC << " seconds" << endl;
	cout << "Speed of reading " << (double) number_of_bits / tooked_time << " bits per clock" << endl;

	system("pause");

	return 0;
}