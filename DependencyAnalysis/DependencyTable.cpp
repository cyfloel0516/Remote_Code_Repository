#include "DependencyTable.h"
#include <algorithm>

using namespace std;


Dependency::Dependency(){}

Dependency::Dependency(string from, string to, string type, string usage, TypeDefinition definition)
{
	this->fromFile = from;
	this->toFile = to;
	this->type = type;
	this->usage = usage;
	this->definition = definition;
}

Dependency::Dependency(string from, string to, string type, string usage)
{
	this->fromFile = from;
	this->toFile = to;
	this->type = type;
	this->usage = usage;
}


void DependencyTable::Push(Dependency newType)
{
	if(this->Find(newType.fromFile, newType.toFile, newType.type, newType.usage) == Size())
		this->_dependencyTable.push_back(newType);
}

void DependencyTable::Delete(size_t index)
{
	this->_dependencyTable.erase(this->_dependencyTable.begin() + index);
}

void DependencyTable::Merge(DependencyTable & table)
{
	for (size_t i = 0; i < table.Size(); i++) {
		this->Push(table[i]);
	}
}

size_t DependencyTable::Size()
{
	return this->_dependencyTable.size();
}

Dependency DependencyTable::operator[](size_t index)
{
	return this->_dependencyTable[index];
}

size_t DependencyTable::Find(std::string from, std::string to, std::string type, std::string usage)
{
	Dependency dep = Dependency(from, to, type, usage);

	auto resultIt = std::find_if(_dependencyTable.begin(), _dependencyTable.end(), [&](Dependency item) {
		return DependencyTable::Equal(dep, item);
	});
	if (resultIt == _dependencyTable.end()) {
		return _dependencyTable.size();
	}
	else
		return resultIt - _dependencyTable.begin();
}

bool DependencyTable::Equal(const Dependency & dep1, const Dependency & dep2)
{
	return dep1.fromFile == dep2.fromFile && dep1.toFile == dep2.toFile
		&& dep1.usage == dep2.usage && dep1.type == dep2.type;
}
