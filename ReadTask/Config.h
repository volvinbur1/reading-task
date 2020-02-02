#pragma once

#include <string>
#include <vector>
using namespace std;

class Config {
public:
	Config(string _path);
	void print_all();
	string title;
	string path;
	vector<string> file_names;
	float speed;
	int amount_of_files;
	bool error = false; // true when an error was occurred while config file was been reading
private:
	void _speed_convert();
	void _path_corect();
};

