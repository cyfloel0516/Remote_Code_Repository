/*
* TypeTable.cpp
* Implementation of TypeDefinition and TypeTable
* This package contains two class:
* 1. TypeDefinition: it is the type definition.
* 2. TypeTable: Holds typedefinitions. it is a container of type definition.
*
* Author: Junkai Yang
*/
#include <string>
#include <vector>
#include <algorithm>
#include "TypeTable.h"
#include "../Tokenizer/Utils.h"
using namespace std;

TypeDefinition::TypeDefinition(){}

// Constructor without wholename parameter
TypeDefinition::TypeDefinition(string name, string type, string nameSpace, string filePath):
name(name), type(type), nameSpace(nameSpace), filePath(filePath){}

// Complete constructor 
TypeDefinition::TypeDefinition(string name, string type, string nameSpace, string filePath, string wholeName):
	name(name), type(type), nameSpace(nameSpace), filePath(filePath) , wholeName(wholeName){
	
}

// A static function which tells if two type definition is equal based on the name, type and namespace
bool TypeTable::Equal(const TypeDefinition& type1, const TypeDefinition& type2) {
	return type1.name == type2.name
		&& type1.type == type2.type
		&& type1.nameSpace == type2.nameSpace;
}

std::string TypeTable::GetRealName(const string &)
{
	return std::string();
}

// Add new type tuple to current type table
void TypeTable::Push(TypeDefinition newType) {
	// Avoid pushing duplicated element into the table
	if (Find(newType.name, newType.type, newType.nameSpace) == Size()) {
		_typeTable.push_back(newType);
	}
}

// Add new type tuple to current type table
void TypeTable::Push(std::string name, std::string type, std::string nameSpace, std::string filePath) {
	auto newType = TypeDefinition(name, type, nameSpace, filePath);
	this->Push(newType);
}

// Add new type tuple to current type table
void TypeTable::Push(std::string name, std::string type, std::string nameSpace, std::string filePath, std::string wholeName) {
	auto newType = TypeDefinition(name, type, nameSpace, filePath, wholeName);
	this->Push(newType);
}

// Delete a type definition from type table by index
void TypeTable::Delete(size_t index) {
	if (index < this->Size())
		_typeTable.erase(_typeTable.begin() + index);
}

// Merge all types to current table
void TypeTable::Merge(TypeTable& table) {
	for (size_t i = 0; i < table.Size(); i++) {
		this->Push(table[i]);
	}
}

// Get the length of type table
size_t TypeTable::Size() { return this->_typeTable.size(); }

// Get the index of type definition by name, type and namespace as arguments
size_t TypeTable::Find(std::string name, std::string type, std::string nameSpace) {
	auto types = utils::split(type, ' ');
	for (auto t : types) {
		TypeDefinition typeDef(name, t, nameSpace, "");
		if (_typeTable.size() == 0)
			return 0;
		auto resultIt = std::find_if(_typeTable.begin(), _typeTable.end(), [&](TypeDefinition item) {
			return TypeTable::Equal(typeDef, item);
		});
		if (resultIt != _typeTable.end()) {
			return resultIt - _typeTable.begin();
		}
	}
	return _typeTable.size();
}

// Get the type definition by index
TypeDefinition TypeTable::operator[](size_t index) {
	return _typeTable[index];
}
