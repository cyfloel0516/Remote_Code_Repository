#pragma once
/*
*	RepositoryDependencyAnalyser.h
*	This package takes care of the dependency analysis and related things for code files.
*
*	Author: Junkai Yang
*/
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
	void InitTypeTable();
	
	// Get dependency of a specified module
	vector<string> GetDependency(string modulePath);
private:
	string modulePath;
	map<string, string> moduleLookupTable;
	TypeTable typeTable;
};