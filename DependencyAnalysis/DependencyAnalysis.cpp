/*
* DependencyAnalysis.cpp 
* Defines the exported functions for the DLL application.
* 
* Author: Junkai Yang
*/
#include "DependencyAnalysis.h"
#include "../AstTree/ASTNode.h"
#include "../MetricsAnalysis/AstAnalyser.h"
#include <functional>

DependencyAnalysis::DependencyAnalysis(){
	_pTypeTable = nullptr;
}

DependencyAnalysis::DependencyAnalysis(TypeTable * table)
{
	this->SetTypeTable(table);
}

void DependencyAnalysis::SetTypeTable(TypeTable * table)
{
	this->_pTypeTable = table;
}

DependencyTable DependencyAnalysis::Analysis(std::string fileName)
{
	auto parserResult = AstAnalyser::analysisFile(fileName);
	DependencyTable table;
	std::string currentNamespace = "";
	std::function<void(ASTNode*)> walkTree;
	walkTree = [&](ASTNode* node) {
		for (auto detector : detectors) {
			auto dep = detector->getDependency(node);
			if (dep.type != "null" && dep.definition.filePath != fileName) {
				dep.fromFile = fileName;
				table.Push(dep);
			}
		}
		for (auto child : node->children) {
			walkTree(child);
		}
	};
	walkTree(parserResult);
	return table;
}

void DependencyAnalysis::AddDetector(IDependencyDetector * detector)
{
	this->detectors.push_back(detector);
}

DependencyAnalysis::~DependencyAnalysis(){}

#ifdef DEPENDENCYANALYSIS_TEST
#include "TypeAnalysis.h"
#include "TypeDetector.h"
#include "TypeTable.h"
void main() {
	ClassDefinition *classDef = new ClassDefinition();
	TypeDefDefinition *typeDef = new TypeDefDefinition();
	AliasDefinition *aliasDef = new AliasDefinition();
	GlobalFunctionDefinition *gFunctionDef = new GlobalFunctionDefinition();
	TypeAnalysis analyser;
	analyser.AddDetector(classDef);
	analyser.AddDetector(typeDef);
	analyser.AddDetector(aliasDef);
	analyser.AddDetector(gFunctionDef);

	auto typeTable = analyser.Analysis("../test_files/test.h");
	for (size_t i = 0; i < result.Size(); i++) {
		cout << result[i].type << " " << result[i].name << " " << result[i].nameSpace << " " << result[i].wholeName << endl;
	}

	DependencyAnalysis dAnalysis;
	InheritDependencyDetector* inheritDetector = new InheritDependencyDetector(typeTable);
	GFunctionDependencyDetector* gFunctionDetector = new GFunctionDependencyDetector(typeTable);
	ClassDependencyDetector* classDetector = new ClassDependencyDetector(typeTable);
	dAnalysis.AddDetector(inheritDetector);
	dAnalysis.AddDetector(gFunctionDetector);
	dAnalysis.AddDetector(classDetector);
	auto result = dAnalysis.Analysis(file);
	cout << "Finished" << endl;
}
#endif