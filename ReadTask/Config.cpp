#include "Config.h"
#include "toml.hpp"
#include <iostream>

Config::Config(string _path) {
	const auto data = toml::parse(_path);

	title = toml::find<string>(data, "title");
	path = toml::find<string>(data, "file", "path");
	file_names = toml::find<vector<string>>(data, "file", "names");
	speed = toml::find<float>(data, "settings", "speed");

	_speed_convert();
}

void Config::print_all() {
	cout << title << endl;
	(path == "") ? cout << __FILE__ << endl : cout << path << endl;

	for (string name : file_names) {
		cout << name << "\t";
	}

	cout << endl << speed << endl;
}

void Config::_speed_convert() {
	if (speed != -1)
		speed *= 1024.f;
}