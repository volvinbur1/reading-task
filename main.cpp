#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <list>
#include <ctime>
#include "toml.hpp"

using namespace std;

class Config {
	public:
		Config(string path);
		void print_all();
		string title;
		string path;
		vector<string> file_names;
		float speed;
		int amount_of_files;
};

Config::Config(string path) {
	const auto data = toml::parse(path);
	title = toml::find<string>(data, "title");
	path = toml::find<string>(data, "file", "path");
	file_names = toml::find<vector<string>>(data, "file", "names");
	speed = toml::find<float>(data, "settings", "speed");
}

void Config::print_all() {
	cout << title << endl;
	(path == "") ? cout << __FILE__ << endl :  cout << path << endl;
	for (string name : file_names) {
		cout << name << "\t";
	}
	cout << endl << speed << endl;
}

bool reading_is_finished = false;
list<string> content;

void read_file_content(Config* config) {
	for (string name : config->file_names) {
		content.push_back("__" + name + "__");

		{
			unique_ptr<ifstream> input(new ifstream);// = make_unique<ifstream>();

			input->open(config->path + name);

			if (input->is_open()) {
				string line;
				while (getline(*input, line))
					content.push_back(line);
			}
		}
	}
	reading_is_finished = true;
}

clock_t start_time = 0, end_time;

void print_time() {
	clock_t now = clock();
	end_time = now;
	string dots = "";

	system("cls");

	cout << "Reading continues" + dots << endl;
	dots += ".";
	cout << "Time has passed " << now - start_time << " seconds" << endl;
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

	cout << "Whole reading took " << (end_time - start_time) / CLOCKS_PER_SEC << " seconds" << endl;

	system("pause");

	return 0;
}