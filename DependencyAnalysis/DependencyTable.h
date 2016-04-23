/*
* DependencyTable.h
* Implementation of DependencyTable.h
* This file contains data structure to store the dependency relation which is the result of dependency analysis
* 
* Author: Junkai Yang
*/
#pragma once

#include <string>
#include <vector>
#include "TypeTable.h"

using namespace std;

// Dependency Struct, holed by dependency table
struct Dependency {
	string fromFile;
	string toFile;
	string type;
	string usage;
	TypeDefinition definition;

	Dependency();
	Dependency(string from, string to, string type, string usage, TypeDefinition definition);
	Dependency(string from, string to, string type, string usage="");
};

// Dependency Table, contains many dependency struct and some useful function
class DependencyTable {
private:
	std::vector<Dependency> _dependencyTable;
public:
	void Push(Dependency newType);
	void Delete(size_t index);
	void Merge(DependencyTable& table);
	size_t Size();
	Dependency operator[](size_t index);
	size_t Find(std::string from, std::string to, std::string type, std::string usage);
	bool static Equal(const Dependency&, const Dependency&);
};