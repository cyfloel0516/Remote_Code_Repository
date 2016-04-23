/*
* ASTNode.h: Implementation of AST node. Each node includes a vector of child nodes to build the structure of tree.
* version: 1.0
* Languange: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
* Source: Jim Fawcett, Syracuse University, CST 4-187
*/

#include "ASTNode.h"

string ASTNode::getFullName()
{
	string result;
	for (auto token : this->tokens) {
		result += token.tokenValue + " ";
	}
	return result;
}

std::string ASTNode::show() {
	std::ostringstream temp;
	temp << type;
	temp << ", ";
	temp << name;
	temp << ", ";
	temp << startLineCount;
	temp << "-";
	temp << endLineCount;
	temp << ", ";
	temp << actualLineCount;
	temp << ", ";
	temp << complexity;
	return temp.str();
}


// Test stub

#ifdef ASTTREE_TEST

int main() {
	ASTNode* root = new ASTNode;
	root->name = "root";
	root->type = "root";
	root->complexity = 1;
	root->startLineCount = 1;
	root->endLineCount = 10;

	ASTNode* children1 = new ASTNode;
	root->name = "testFunc";
	root->type = "function";
	root->complexity = 1;
	root->startLineCount = 3;
	root->endLineCount = 5;

	ASTNode* children2 = new ASTNode;
	root->name = "if";
	root->type = "if";
	root->complexity = 5;
	root->startLineCount = 5;
	root->endLineCount = 10;

	root->children.push_back(children1);
	root->children.push_back(children2);

	root->show();
	children1->show();
	children2->show();
}
#endif // ASTTREE_TEST
