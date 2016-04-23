/*
* AnalysisHeler.cpp
* IImplementation of AnalysisHelper.h.
* 
* Author: Junkai Yang
*/
#include <string>
#include <vector>
#include "AnalysisHelper.h"
#include "../Tokenizer/Utils.h"

using namespace std;

string AnalysisHelper::getNamespace(ASTNode * node)
{
	if (node == nullptr)
		return "Global";
	if (node->type == "namespace")
		return node->name;
	auto wholeName = node->getFullName();
	auto tokens = utils::split(wholeName, ' ');
	auto splitIndex = std::find(tokens.begin(), tokens.end(), "::");
	if (splitIndex != tokens.end()) {
		return *(splitIndex - 1);
	}
	else {
		string ns = "Global";
		auto current = node->parent;
		while (current != nullptr) {
			if (current->type == "namespace") {
				return current->name;
			}
			current = current->parent;
		}
		return ns;
	}
}

#ifdef DEPENDENCYANALYSIS_H
int main() {
	
}
#endif