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
private:
	void _speed_convert();
};

