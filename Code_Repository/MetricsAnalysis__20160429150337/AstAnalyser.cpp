/*
* AstAnalyser.h: implementation of AST Analyser.
* version: 1.0
* Languange: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
*/
// Ast Analyser includes the functions of analyse the actual line numbers and complexity of AST tree.
// It also includes the function to print the AST tree.

#include <iostream>
#include <functional>
#include "AstAnalyser.h"
#include "../AstTree/ASTNode.h"
#include "../Parser/ConfigureParser.h"
#include "../FileSystem_Windows/FileSystem.h"
#include <iomanip>

void AstAnalyser::analysis(ASTNode* root){
	if (root->type == "function") {
		root->complexity = 1;
	}
	if (root->children.size() > 0) {
		auto it = root->children.begin();
		while (it != root->children.end()) {
			analysis(*it);
			it++;
		}
	}
	computeComplexity(root);
	computeLineNumber(root);
	if (root->parent != nullptr) {
		root->parent->complexity += root->complexity;
		root->parent->actualLineCount += root->actualLineCount;
	}
}

ASTNode* AstAnalyser::analysisFile(std::string& filePath) {
	// Use parser to build AST tree first.
	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();
	try
	{
		if (pParser)
		{
			if (!configure.Attach(filePath))
			{
				std::cout << "\n  could not open file " << filePath << std::endl;
				return nullptr;
			}
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return nullptr;
		}

		while (pParser->next())
			pParser->parse();
		AstAnalyser::analysis(configure.pRepo->getRoot());
		return configure.pRepo->getRoot();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	return nullptr;
}

void AstAnalyser::computeLineNumber(ASTNode* node) {
	if (node->children.size() == 0 &&
		( (node->type != "if" && node->type != "while" && node->type != "switch" && node->type != "for")
			||
			((node->type == "if" || node->type == "while" || (node->type == "switch") || node->type == "for") && node->startWith == "{")
		)
	) {
		//is leaf node
		node->actualLineCount = node->endLineCount - node->startLineCount + 1;
	}
}

void AstAnalyser::computeComplexity(ASTNode* node) {
	//is leaf node
	if (node->type == "if" || node->type == "while" || (node->type == "switch") || node->type == "for") {
		node->complexity += 1;
	}
}

void AstAnalyser::printTree(ASTNode* node) {
	std::function<void(ASTNode* , int , int , int )> walkTree;
	// Lambda for walking the tree recursively
	walkTree = [&](ASTNode* pItem, int level, int index, int length) {
		if (pItem->type == "Global"
			|| pItem->type == "namespace"
			|| pItem->type == "try"
			|| pItem->type == "class"
			|| pItem->type == "struct"
			|| pItem->type == "function"
			|| pItem->type == "if"
			|| pItem->type == "switch"
			|| pItem->type == "while"
			|| pItem->type == "for"
			|| pItem->type == "simple scope") 
		{	
			std::cout << "\n";
			if (level != 0)
				std::cout << "|";

			std::string separator;
			if (index == length - 1)
				separator = "`--";
			else
				separator = "|--";
			cout << std::string(3 * level, ' ') << separator << " " << pItem->show();
		}
		auto iter = pItem->children.begin();
		auto size = pItem->children.size();
		++level;
		while (iter != pItem->children.end())
		{
			walkTree(*iter, level, iter - pItem->children.begin(), size);
			++iter;
		}
	};
	walkTree(node, 0, 0, node->children.size());
}

void AstAnalyser::printSummary(ASTNode* node) {
	int totalLines= 0;
	int totalActualLines = 0;
	int maxComplexity = 0;
	int totalComplexity = 0;
	string maxComplexityFunction;
	std::function<void(ASTNode*, int, int, int)> walkTree;
	// Lambda for walking the tree recursively
	walkTree = [&](ASTNode* pItem, int level, int index, int length) {
		if (pItem->type == "function")
		{
			std::cout << std::setw(10) << pItem->complexity 
				<< std::setw(8) << (pItem->endLineCount - pItem->startLineCount + 1)
				<< std::setw(15) << pItem->actualLineCount  << "  "
				<< std::setw(70) << pItem->name << endl;

			if (pItem->complexity > maxComplexity){
				maxComplexity = pItem->complexity;
				maxComplexityFunction = pItem->name;
			}
			totalLines += (pItem->endLineCount - pItem->startLineCount + 1);
			totalActualLines += pItem->actualLineCount;
			totalComplexity += pItem->complexity;
		}

		auto iter = pItem->children.begin();
		auto size = pItem->children.size();
		++level;
		while (iter != pItem->children.end())
		{
			walkTree(*iter, level, iter - pItem->children.begin(), size);
			++iter;
		}
	};
	std::cout << endl;
	std::cout << std::setw(10) << "complexity"
		<< std::setw(8) << "lines"
		<< std::setw(15) << "actual lines" << "  "
		<< std::setw(70) << "function names" << endl;
	walkTree(node, 0, 0, node->children.size());
	std::cout << endl;
	std::cout << "lines of functions text: " << totalLines << endl;
	std::cout << "lines of function code:  " << totalActualLines << endl;
	std::cout << "total complexity:        " << totalComplexity << endl;
	std::cout << "max complexity:          " << maxComplexity << " (" << maxComplexityFunction << ")" << endl;
}

//----< test stub >--------------------------------------------
#ifdef ASTANALYSER_CPP
int main() {
	std::string fileSpec = FileSystem::Path::getFullFileSpec("../MetricsAnalysis/AstAnalyser.cpp");
	auto root = AstAnalyser::analysisFile(fileSpec);
	AstAnalyser::printTree(root);
	AstAnalyser::printSummary(root);
	return 1;
}
#endif