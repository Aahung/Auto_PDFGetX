#include "Utility.h"

std::vector<std::string> Utility::split_str(std::string input, std::string delimiter) {
	std::vector<std::string> output;
	int input_len = input.size();
	int del_len = delimiter.size();
	std::size_t start = 0;
	std::size_t pos;
	do {
		pos = input.find(delimiter, start);
		if (pos != start) {
			output.push_back(input.substr(start, pos - start));
		}
		start = pos + delimiter.length();
	} while (pos != std::string::npos);
	return output;
}

char * Utility::string2char(std::string str)
{
	size_t size = str.size();
	char * ch = new char[size + 1];
	for (int i = 0; i < size; i++)
	{
		ch[i] = str[i];
	}
	ch[size] = '\0';
	return ch;
}