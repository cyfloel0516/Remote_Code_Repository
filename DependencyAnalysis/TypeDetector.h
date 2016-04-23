#ifndef TYPEDETECTOR_H
#define TYPEDETECTOR_H
#pragma once
/*
* This is a detector interface for type analysis.
* The implementation of this interface test if a ASTnode is 
* a particular type definition and return the result.
* 
* Author: Junkai Yang
*/
#include "TypeTable.h"
#include "../AstTree/ASTNode.h"
#include "TypeTable.h"
#include "../Tokenizer/Utils.h"
#include <complex>

// Type detector interface
// All detectors implement this interface and called by analyser
class ITypeDetector {
public:
	virtual bool test(ASTNode*& node) = 0;
	// Return type and name as the result.
	virtual TypeDefinition getData(ASTNode*& node) = 0;
};

// Detector to find class definition
class ClassDefinition: public ITypeDetector{
public:
	// Inherited via ITypeDetector
	virtual bool test(ASTNode*& node) override;
	virtual TypeDefinition getData(ASTNode*& node) override;
};

// Detector to find typedef definition
class TypeDefDefinition : public ITypeDetector {
public:
	// Inherited via ITypeDetector
	virtual bool test(ASTNode *& node) override;
	virtual TypeDefinition getData(ASTNode *& node) override;
};

// Detector to find alias definition
class AliasDefinition : public ITypeDetector {
public:
	// Inherited via ITypeDetector
	virtual bool test(ASTNode *& node) override;
	virtual TypeDefinition getData(ASTNode *& node) override;
};

// Detector to find global function definition
class GlobalFunctionDefinition : public ITypeDetector {
private:
	bool isLambda(ASTNode *& node);
public:
	// Inherited via ITypeDetector
	virtual bool test(ASTNode *& node) override;
	virtual TypeDefinition getData(ASTNode *& node) override;

};
#endif

