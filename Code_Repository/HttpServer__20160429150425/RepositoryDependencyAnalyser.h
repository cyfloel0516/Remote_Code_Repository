/*
*	RepositoryDependencyAnalyser.h
*	This package takes care of the dependency analysis and related things for code files.
*	To use it, fist call InitTypeTable to initialize the type table and then use GetDependency to get dependency relation.
*
*	Author: Junkai Yang
*/
#pragma once
#include "../ParallelDependencyExecutive/ParallelExecutive.h"
#include "RepositoryMetadata.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

class RepositoryDependencyAnalyser {
public:
	// Initialize the type table
	// This function go thought every closed package and get the type table.
	void InitTypeTable(string moduleName);
	
	// Get dependency of a specified module
	vector<string> GetDependency(string modulePath);
private:
	string modulePath;
	map<string, string> moduleLookupTable;
	TypeTable typeTable;
};