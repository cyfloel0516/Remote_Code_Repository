/*
* ASTNode.h: Definition of AST node. Each node includes a vector of child nodes to build the structure of tree.
* version: 1.0
* Languange: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
* Source: Jim Fawcett, Syracuse University, CST 4-187
*/

#ifndef ASTNODE_H
#define ASTNODE_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "../Tokenizer/Tokenizer.h"

// ASTNode class
struct ASTNode
{
	string getFullName();
	// Node type
	std::string type;

	// Node name
	std::string name;
	
	// the token of the node
	Scanner::Token token;

	// Line number information
	size_t startLineCount = 0;
	size_t endLineCount = 0;

	// some data support further modification for scope without bracket
	std::string startWith = "";
	std::string endWith = "";

	// Actual line number, calculated by AstAnalyser
	int actualLineCount = 0;

	// Complexity, calculated by AstAnalyser
	int complexity = 0;

	// Its parent node
	ASTNode* parent;

	// The vector of its children
	std::vector<ASTNode*> children;
	std::vector<Scanner::Token> tokens = std::vector<Scanner::Token>();
	
	// A simple function to show itself in default style
	std::string show();
};

#endif
