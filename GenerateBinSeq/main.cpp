#include <iostream>
#include <stdio.h>
#include <memory>
#include <string>
#include <bitset>
#include <ctime>
#pragma warning(disable : 4996)

using namespace std;

uint64_t write_binary(char* path) {
	uint64_t number_of_bits = 0;

	auto stream = unique_ptr<FILE, decltype(&fclose)>(fopen(path, "wb"), &fclose);
	
	if (stream != NULL) {
		for (uint32_t i = 0; i < UINT32_MAX / pow(2, 16); i++) {
			//char binary_array[32];
			string binary = bitset<32>(i).to_string();
			//strcpy(binary_array, binary.c_str());
			cout << binary.c_str() << endl;
			fwrite(&i, sizeof(uint32_t), 1, stream.get());
			fseek(stream.get(), sizeof(uint32_t), SEEK_CUR);
			//fputs(binary.c_str(), stream.get());
			number_of_bits += 32;
		}
	}

	return number_of_bits;
}

int main() {
	char path[] = "f.bin";

	clock_t start_time = clock();
	auto number_of_bits = write_binary(path);
	
	clock_t amount_of_clocks = clock() - start_time;

	cout << "Taken time " << (double) amount_of_clocks / CLOCKS_PER_SEC << " seconds" << endl;
	cout << "Speed of writing " << (double)number_of_bits / amount_of_clocks << " bits per clock" << endl;

	system("pause");	
	return 0;
}