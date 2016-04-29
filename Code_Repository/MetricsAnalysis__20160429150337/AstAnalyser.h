/*
* AstAnalyser.h: Definition of AST Analyser.
* version: 1.0
* Languange: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
*/
// Ast Analyser includes the functions of analyse the actual line numbers and complexity of AST tree.
// It also includes the function to print the AST tree.

#ifndef ASTANALYSER_H
#define ASTANALYSER_H

#include "../AstTree/ASTNode.h"

class AstAnalyser {
public:
	// Accept AST tree node as parameter and calculate the line numbers and complexity of the node and all of its children
	static void analysis(ASTNode* root);

	// Same to the analysis function. The only difference with analysis function is that this function accept source code file path as parameter
	// and use parser to build AST tree first.
	static ASTNode* analysisFile(std::string& filePath);

	// Print AST tree, keep the tree structure in output.
	static void printTree(ASTNode* node);

	// Print the AST summary in the list view, now it will only print the function node.
	static void printSummary(ASTNode* node);
private:
	static void computeLineNumber(ASTNode* node);
	static void computeComplexity(ASTNode* node);
};

#endif