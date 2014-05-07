#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace Utility {
	std::vector<std::string> split_str(std::string input, std::string delimiter) {
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
}