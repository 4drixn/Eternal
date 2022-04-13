#pragma once
#include <string>

namespace util
{
	// stackoverflow utils

	std::vector<std::string> split(std::string text, char delim) {
		std::string line;
		std::vector<std::string> vec;
		std::stringstream ss(text);
		while (std::getline(ss, line, delim)) {
			vec.push_back(line + delim);
		}
		return vec;

	}
	
	std::string get_str_between_two_str(const std::string& s,
		const std::string& start_delim,
		const std::string& stop_delim)
	{
		unsigned first_delim_pos = s.find(start_delim);
		unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
		unsigned last_delim_pos = s.find(stop_delim);

		return s.substr(end_pos_of_first_delim,
			last_delim_pos - end_pos_of_first_delim);
	}

}
