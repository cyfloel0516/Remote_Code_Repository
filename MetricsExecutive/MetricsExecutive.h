/*
* MetricsExecutive.h: header file of metrics executive
* version: 1.0
* Languange: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
*/
// Metrics Executive accept a bunch of files and use the AST analyser to analyse them.
// Then you can use print function to print the result to console.
#include <string>
#include <iostream>
#include <vector>
#include "../MetricsAnalysis/AstAnalyser.h"
#include "../FileSystem_Windows/FileSystem.h"

class MetricsExecutive {
public:
	// Accept a list of file path, analyse these files and print the ast tree to console
	static void PrintMetrics(std::vector<std::string>& filePaths);
};