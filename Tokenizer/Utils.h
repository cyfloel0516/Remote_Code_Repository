/*
* Utils.h: contains some useful utils
* Version: 1.0
* Language: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
*/

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

namespace utils {
	// Split a string based on a specified character
	 std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

	// Split a string based on a specified character
	 std::vector<std::string> split(const std::string &s, char delim);

	// Trim left part of a string
	 std::string trimL(const std::string &s);

	 // Replace some content in a string with other content
	 std::string replace(const std::string& str, const std::string& from, const std::string& to);

	 // Change a string into escaped string
	 std::string escape(const std::string& str);

	 // Change a escaped string back to un escaped style
	 std::string un_escape(const std::string& str);

	 void Title(const std::string& std);
}

#endif