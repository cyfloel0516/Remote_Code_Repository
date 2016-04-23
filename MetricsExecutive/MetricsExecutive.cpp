/*
* MetricsExecutive.h: implementation of metrics executive
* version: 1.0
* Languange: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
*/
// Metrics Executive accept a bunch of files and use the AST analyser to analyse them.
// Then you can use print function to print the result to console.

#include "MetricsExecutive.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "../MetricsAnalysis/AstAnalyser.h"
#include "../AstTree/ASTNode.h"
#include "../FileSystem_Windows/FileSystem.h"
#include "../FileSystem_Windows/FileSystemSearchHelper.h"
#include <windows.h>

void MetricsExecutive::PrintMetrics(std::vector<std::string>& filePaths) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::for_each(filePaths.begin(), filePaths.end(), [&](std::string& path) {
		path = FileSystem::Path::getFullFileSpec(path);
		SetConsoleTextAttribute(hConsole, 14);
		cout << "Analysing file: " << path << "..." << endl;
		SetConsoleTextAttribute(hConsole, 7);
		path = FileSystem::Path::getFullFileSpec(path);
		auto root = AstAnalyser::analysisFile(path);
		if (root != nullptr) {
			AstAnalyser::printTree(root);
			SetConsoleTextAttribute(hConsole, 14);

			cout << endl <<
				"---------------------------------------------------------------------------------" << endl;
			cout << "following is the metrics summary for file: " << path << "..." << endl;
			SetConsoleTextAttribute(hConsole, 7);
			AstAnalyser::printSummary(root);
			SetConsoleTextAttribute(hConsole, 14);
			cout << "=================================================================================" << endl;
			cout << "=================================================================================" << endl;
			cout << endl;
		}
		
	});
}

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
void runAstTreeBuildTest() {
	std::string userInput;
	std::string folderPath = "../Parser";
	std::string pattern;
	cout << "===================================================================================" << endl;
	std::cout << "This default test will go through all .cpp and .h files under /Parser folder " << endl;
	std::cout << "and build the AST tree for each of these files." << endl;
	std::cout << "The output format is <type, name, start-end, actual lines, complexity>" << endl;
	std::cout << endl;
	std::cout << "This test can show you my program satisfy the requirements 2,3,4,5,6,7,8,9,10 by: " << endl;
	std::cout << "1. Build AST tree: you can see AST tree structure in every first part of analysing." << endl;
	std::cout << "2. The metrics analysis in every second part of analysing." << endl;
	std::cout << "3. File search function(You can run more searching test use FileSearch_Test.exe)." << endl;
	std::cout << endl;
	cout << "You can see all cases: if, else, try, catch, switch, for, while, function, lambda, namespace, class, struct in the" << endl;
	cout << "in TestFile.h (last file in the following list)." << endl;
	cout << endl;
	cout << "===================================================================================" << endl;

	pattern = "*.cpp";
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "1. Building AST tree for .cpp file under /Parser folder." << endl;
	FileSystem::Directory::setCurrentDirectory(folderPath);
	auto files = FileSystemSearchHelper::searchFiles(FileSystem::Directory::getCurrentDirectory(), pattern);
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "Begin searching in " << "" << " with pattern " << pattern << "..." << endl;
	std::cout << "======================================================================" << endl;
	std::cout << "Matched files in this folder: " << endl;
	std::for_each(files.begin(), files.end(), [](std::string& path) {
		std::cout << FileSystem::Path::getFullFileSpec(path) << endl;
	});
	std::cout << "======================================================================" << endl;
	std::cout << "======================================================================" << endl;
	MetricsExecutive::PrintMetrics(files);

	pattern = "*.h";
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "2. Building AST tree for all .h files in /Parser folder.." << endl;
	FileSystem::Directory::setCurrentDirectory(folderPath);
	files = FileSystemSearchHelper::searchFiles(FileSystem::Directory::getCurrentDirectory(), pattern);
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "Begin searching in " << "" << " with pattern " << pattern << "..." << endl;
	std::cout << "======================================================================" << endl;
	std::cout << "Matched files in this folder: " << endl;
	std::for_each(files.begin(), files.end(), [](std::string& path) {
		std::cout << FileSystem::Path::getFullFileSpec(path) << endl;
	});
	std::cout << "======================================================================" << endl;
	std::cout << "======================================================================" << endl;
	MetricsExecutive::PrintMetrics(files);

	SetConsoleTextAttribute(hConsole, 7);
	std::cout << endl;
	cout << "You can see all cases: if, else, try, catch, switch, for, while, function, lambda, namespace, class, struct in the" << endl;
	cout << "in TestFile.h (last file in the above list)." << endl;
	cout << "You can refer to readme.txt and run your customized test with folder and search pattern." << endl;
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "Press enter to exit..." << endl;
	std::getline(std::cin, userInput);
}

//Test program
int main(int argc, char* argv[]) {
	std::string userInput;
	if (argc == 1)
	{
		runAstTreeBuildTest();
		return 0;
	}
	else if (argc == 2) {
		//Run test for a single file
		std::string fileSpec = FileSystem::Path::getFullFileSpec(argv[1]);
		auto fileInfo = FileSystem::FileInfo(fileSpec);
		if (fileInfo.isDirectory() || !FileSystem::File::exists(fileSpec)) {
			cout << "Please input the correct file name!" << endl;
			return 1;
		}
		else {
			SetConsoleTextAttribute(hConsole, 10);
			std::vector<std::string> files = { fileSpec };
			MetricsExecutive::PrintMetrics(files);
			cout << endl;
			SetConsoleTextAttribute(hConsole, 10);
			std::cout << "Press enter to exit..." << endl;
			std::getline(std::cin, userInput);
			return 1;
		}
	}
	else if (argc == 3) {
		//Run test for a single file
		std::string folderPath = FileSystem::Path::getFullFileSpec(argv[1]);
		std::string pattern = std::string(argv[2]);
		auto fileInfo = FileSystem::FileInfo(folderPath);
		if (!fileInfo.isDirectory() || !FileSystem::File::exists(folderPath)) {
			cout << "Please input the correct folder name!" << endl;
			return 1;
		}
		else {
			SetConsoleTextAttribute(hConsole, 10);
			std::vector<std::string> files = FileSystemSearchHelper::searchFiles(folderPath, pattern);
			FileSystem::Directory::setCurrentDirectory(folderPath);
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "Begin searching in " << folderPath << " with pattern " << pattern << "..." << endl;
			std::cout << "======================================================================" << endl;
			std::cout << "Matched files in this folder: " << endl;
			std::for_each(files.begin(), files.end(), [](std::string& path) {
				std::cout << FileSystem::Path::getFullFileSpec(path) << endl;
			});
			std::cout << "======================================================================" << endl;
			std::cout << "======================================================================" << endl;
			std::cout << endl;
			MetricsExecutive::PrintMetrics(files);
			cout << endl;
			SetConsoleTextAttribute(hConsole, 10);

			std::cout << "Press enter to exit..." << endl;
			std::getline(std::cin, userInput);
			return 1;
		}
	}

}


