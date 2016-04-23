#include "DependencyDetector.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Tokenizer/Utils.h"
#include "AnalysisHelper.h"
#include <vector>
#include <string>

using namespace Scanner;

// This method can extract the real base name of inheritance
string InheritDependencyDetector::getBaseClassName(ASTNode *& node)
{
	string fullName = node->getFullName();
	auto tokens = utils::split(fullName, ' ');
	auto colonIndex = std::find(tokens.begin(), tokens.end(), ":");
	auto hasNamespace = std::find(tokens.begin(), tokens.end(), "::") != tokens.end();
	
	if (colonIndex != tokens.end()) {
		auto nextString = *(colonIndex + 1);
		if (nextString == "public" || nextString == "protected" || nextString == "private") {
			if (hasNamespace)
				return *(colonIndex + 4);
			else
				return *(colonIndex + 2);
		}
		else {
			if (hasNamespace)
				return *(colonIndex + 3);
			else
				return nextString;
		}
	}
	return "";
}

string InheritDependencyDetector::getBaseClassNamespace(ASTNode *& node)
{
	string fullName = node->getFullName();
	auto tokens = utils::split(fullName, ' ');
	auto splitIndex = std::find(tokens.begin(), tokens.end(), "::");
	auto hasNamespace =  splitIndex != tokens.end();

	if (hasNamespace) {
		return *(splitIndex - 1);
	}
	else {
		return AnalysisHelper::getNamespace(node);
	}
}

InheritDependencyDetector::InheritDependencyDetector(TypeTable * typeTable)
{
	this->pTypeTable = typeTable;
}

Dependency InheritDependencyDetector::getDependency(ASTNode *& node)
{
	string fullName = node->getFullName();
	string realName = this->getBaseClassName(node);
	
	if (node->type == "class"
		&& fullName.find(":") != string::npos) {
		auto ns = this->getBaseClassNamespace(node);
		auto matchedTypeIndex = this->pTypeTable->Find(realName, "class", ns);
		if (matchedTypeIndex != this->pTypeTable->Size()) {
			TypeDefinition typeDefinition = (*(this->pTypeTable))[matchedTypeIndex];
			Dependency dependency = Dependency();
			dependency.type = "inherit";
			dependency.usage = node->name;
			dependency.definition = typeDefinition;
			dependency.toFile = typeDefinition.filePath;
			return dependency;
		}
	}
	return Dependency("", "", "null", "");
}

GFunctionDependencyDetector::GFunctionDependencyDetector(TypeTable * typeTable)
{
	this->pTypeTable = typeTable;
}

Dependency GFunctionDependencyDetector::getDependency(ASTNode *& node)
{
	string fullName = node->getFullName();
	//auto tokens = utils::split(fullName, ' ');
	string currentNs = AnalysisHelper::getNamespace(node->parent);
	//auto splitIndex = std::find(tokens.begin(), tokens.end(), "=");
	if (node->type == "declaration" || node->type == "executable") {
		for (size_t i = 0; i < node->tokens.size(); i++) {
			auto token = node->tokens[i];
			if (token.tokenType == TOKEN_TYPES::ALPHANUM) {
				// The aplhanum followed by ( or < and no precending . is treated as global function
				//auto gFunctionIndex = this->pTypeTable->Find(token.tokenValue, "function", ns);
				bool isFuncCall = (i < node->tokens.size() - 1) && (node->tokens[i + 1].tokenValue == "<" || node->tokens[i + 1].tokenValue == "(");
				bool isGlobalFunc = (i == 0) || ((i > 0) && node->tokens[i - 1].tokenValue != ".");
				if (isFuncCall && isGlobalFunc) {
					string funcName = token.tokenValue;
					string ns = currentNs;
					if (i > 1) {
						if (node->tokens[i - 1].tokenValue == "::") {
							ns = node->tokens[i - 2].tokenValue;
						}
					}
					auto gFunctionIndex = this->pTypeTable->Find(funcName, "function", ns);
					if (gFunctionIndex != this->pTypeTable->Size()) {
						auto def = (*(this->pTypeTable))[gFunctionIndex];
						return Dependency("", def.filePath, "global_function", funcName, def);
					}
				}
			}
		}
	}
	return Dependency("", "", "null", "");
}

ClassDependencyDetector::ClassDependencyDetector(TypeTable * typeTable)
{
	this->pTypeTable = typeTable;
}

Dependency ClassDependencyDetector::getDependency(ASTNode *& node)
{
	string fullName = node->getFullName();
	string currentNs = AnalysisHelper::getNamespace(node->parent);
	//auto splitIndex = std::find(tokens.begin(), tokens.end(), "=");

	// Go through each token and see if they are one of the class definitions in type table
	// Also need to check the namespace
	for (size_t i = 0; i < node->tokens.size(); i++) {
		auto token = node->tokens[i];
		if (token.tokenType == TOKEN_TYPES::ALPHANUM) {
			string ns = currentNs;
			if (i > 1) {
				if (node->tokens[i - 1].tokenValue == "::") {
					ns = node->tokens[i - 2].tokenValue;
				}
			}
			auto gFunctionIndex = this->pTypeTable->Find(token.tokenValue, "class enum struct using typedef", ns);
			if (gFunctionIndex != this->pTypeTable->Size()) {
				auto def = (*(this->pTypeTable))[gFunctionIndex];
				return Dependency("", def.filePath, def.type, token.tokenValue, def);
			}
		}
	}
	
	return Dependency("", "", "null", "");
}
