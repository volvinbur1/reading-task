#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include "toml.hpp"

using namespace std;

class Config {
	public:
		Config(string path);
		void print_all();
	private:
		string _title;
		string _path;
		vector<string> _file_names;
		float _speed;
		int _amount_of_files;
};

Config::Config(string path) {
	const auto data = toml::parse(path);
	_title = toml::find<string>(data, "title");
	_path = toml::find<string>(data, "file", "path");
	_file_names = toml::find<vector<string>>(data, "file", "names");
	_speed = toml::find<float>(data, "settings", "speed");
}

void Config::print_all() {
	cout << _title << endl;
	(_path == "") ? cout << __FILE__ << endl :  cout << _path << endl;
	for (string name : _file_names) {
		cout << name << "\t";
	}
	cout << endl << _speed << endl;
}

//Config read_config() {
//	Config output;
//
//	const auto data = toml::parse("config.toml");
//	output.title = toml::find<string>(data, "title");
//	output.path = toml::find<string>(data, "path");
//	output.file_names = toml::find<vector<string>>(data, "names");
//	output.speed = toml::find<float>(data, "speed");
//
//	{
//		unique_ptr<ifstream> input(new ifstream);// = make_unique<ifstream>();
//		
//		input->open("config.toml");
//		
//		if (input->is_open()) {
//			string line;
//			while (getline(*input, line))
//				output.push_back(line);
//		}
//	}
//	return output;
//}



int main() {
	Config config("config.toml");

	config.print_all();

	system("pause");

	return 0;
}