#ifndef TYPETABLE_H
#define TYPETABLE_H
/*
* TypeTable.h
* This package contains two class:
* 1. TypeDefinition: it is the type definition.
* 2. TypeTable: Holds typedefinitions. it is a container of type definition.
*
* Author: Junkai Yang
*/
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// Type definition struct
struct TypeDefinition {
	string name;
	string type;
	string nameSpace;
	string filePath;
	string wholeName;

	TypeDefinition();
	TypeDefinition(string, string, string, string);
	TypeDefinition(string, string, string, string, string);
};

// Type definition table, contains multiple type definition
class TypeTable {
private:
	std::vector<TypeDefinition> _typeTable;
public:
	void Push(TypeDefinition newType);
	void Push(std::string name, std::string type, std::string nameSpace, std::string filePath);
	void Push(std::string name, std::string type, std::string nameSpace, std::string filePath, std::string wholeName);
	void Delete(size_t index);

	// Merge two table into one
	void Merge(TypeTable& table);
	size_t Size();
	TypeDefinition operator[](size_t index);
	size_t Find(std::string name, std::string type, std::string nameSpace = "");
	bool static Equal(const TypeDefinition&, const TypeDefinition&);
	std::string static GetRealName(const string&);
};

#endif