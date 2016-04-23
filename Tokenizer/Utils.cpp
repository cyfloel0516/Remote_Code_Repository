/*
* Utils.cpp: Implementation of utils
* Version: 1.0
* Application: Visual Studio 2015
* Author: Junkai Yang
*/

#include "Utils.h"
#include <vector>
#include <sstream>
#include <windows.h>
#include <iostream>

namespace utils {
	std::string trimL(const std::string &s) {
		if (!s.empty()){
			int first = s.find_first_not_of("\n ");
			if (first >= 0) {
				return s.substr(first);
			}
		}
		return s;
	}

	std::vector<std::string>  &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	std::string replace(const std::string& str, const std::string& from, const std::string& to) {
		auto copy = std::string(str);
		size_t start_pos = copy.find(from);
		if (start_pos != std::string::npos)
			copy.replace(start_pos, from.length(), to);
		return copy;
	}

	std::string escape(const std::string& str) {
		auto copy = std::string(str);
		copy = utils::replace(copy, "\\n", "\n");
		copy = utils::replace(copy, "\\t", "\t");
		copy = utils::replace(copy, "\\\\", "\\");
		return copy;
	}

	std::string un_escape(const std::string& str) {
		auto copy = std::string(str);

		copy = utils::replace(copy, "\\", "\\\\");
		copy = utils::replace(copy,  "\n", "\\n");
		copy = utils::replace(copy, "\t",  "\n");

		return copy;
	}
	void Title(const std::string & std)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << std << "\n";
		SetConsoleTextAttribute(hConsole, 7);
	}
}
