#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <list>

using namespace std;

list<string> read_config() {
	list<string> output;
	{
		unique_ptr<ifstream> input(new ifstream);// = make_unique<ifstream>();
		
		input->open("config.toml");
		
		if (input->is_open()) {
			string line;
			while (getline(*input, line))
				output.push_back(line);
		}
	}
	return output;
}

int main() {
	list<string> config = read_config();
	cout << config.size() << endl;

	for (string str : config) {
		cout << str << endl;
	}

	system("pause");

	return 0;
}