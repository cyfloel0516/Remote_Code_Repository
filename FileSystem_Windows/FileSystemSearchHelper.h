/*
* FileSystemSearchHelper.h: header file of class FileSystemSearchHelper.
* version: 1.0
* Languange: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
*/

#ifndef FILESYSTEMSEARCHHELPER_H
#define FILESYSTEMSEARCHHELPER_H

#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include "FileSystem.h"

// This class includes two static functions which can help to search files or directories
// recursively in specified folder.
// Both will return a string vector with full path of matched item.
class FileSystemSearchHelper {
public:
	// This function will accept multiple patterns and return the search results
	static std::vector<std::string> searchFiles(std::string rootPath, std::vector<std::string> patterns);
	
	// This function will return a string vector with full path of matched item.
	static std::vector<std::string> searchFiles(std::string rootPath, std::string pattern);

	// This function will accept multiple patterns and return the search results
	static std::vector<std::string> searchDirectories(std::string rootPath, std::vector<std::string> pattern);

	// This function will return a string vector with full path of matched item.
	static std::vector<std::string> searchDirectories(std::string rootPath, std::string pattern);
};

#endif