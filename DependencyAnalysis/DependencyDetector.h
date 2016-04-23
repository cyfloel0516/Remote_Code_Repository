/*
* DependencyDetector.h
* This file contains detector definitions which is used for detecting the 
* dependency relation between files.
* Each detector is responsible for one type of dependency relation.
*
* Author: Junkai Yang
*/
#pragma once
#include "TypeTable.h"
#include "DependencyTable.h"
#include "../AstTree/ASTNode.h"
#include "../Tokenizer/Utils.h"
#include "../AstTree/ASTNode.h"

// Dependency Detector interface, all detectors implement this interfaces and called by analyser
class IDependencyDetector {
public:
	virtual ~IDependencyDetector() {}
	// Return type and name as the result.
	virtual Dependency getDependency(ASTNode*& node) = 0;

};

// Inheritance Dependency Detector:
// Detect the inheritance class dependency.
class InheritDependencyDetector: public IDependencyDetector {
private:
	string filename;
	TypeTable* pTypeTable;
	// This method can extract the real base name of inheritance
	string getBaseClassName(ASTNode *& node);
	string getBaseClassNamespace(ASTNode *& node);
public:
	InheritDependencyDetector( TypeTable* typeTable);
	// Inherited via IDependencyDetector
	virtual Dependency getDependency(ASTNode *& node) override;
};

// Global Function Dependency Detector:
// Detect the global function invocation dependency
class GFunctionDependencyDetector : public IDependencyDetector {
private:
	TypeTable* pTypeTable;
	
public:
	GFunctionDependencyDetector(TypeTable* typeTable);
	virtual Dependency getDependency(ASTNode *& node) override;
};

// Class Function Dependency Detector:
// Detect the class reference in code such like template
class ClassDependencyDetector : public IDependencyDetector {
private:
	TypeTable* pTypeTable;

public:
	ClassDependencyDetector(TypeTable* typeTable);
	virtual Dependency getDependency(ASTNode *& node) override; 
	
};

