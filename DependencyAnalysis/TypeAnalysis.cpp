#include "TypeAnalysis.h"
#include "TypeTable.h"
#include <algorithm>
#include <functional>
#include "../MetricsAnalysis/AstAnalyser.h"



// Do analysis
// Get AST root node by AST analysis
// Go through all the node in tree
// Use Detector to test and do action if it is a type definition
TypeTable TypeAnalysis::Analysis(std::string filePath) {
	auto parserResult = AstAnalyser::analysisFile(filePath);
	TypeTable table;
	std::string currentNamespace = "";
	std::function<void(ASTNode*)> walkTree;
	walkTree = [&](ASTNode* node) {
		for (auto detector : detectors) {
			// Remember the current namespace scope
			if (node->type == "namespace") {
				currentNamespace = node->name;
			}
			// Detect the type definition and do action
			if (detector->test(node)) {
				auto type = detector->getData(node);
				//if (type.type == "function" && type.name == "main") {
				//	continue;
				//}
				type.filePath = filePath;
				type.nameSpace = currentNamespace;
				// Make type definition and add it to type table
				table.Push(type);
			}
		}
		for (auto child : node->children) {
			walkTree(child);
		}
		if (node->type == "namespace") {
			currentNamespace = "Global";
		}
	};
	walkTree(parserResult);
	return table;
}

// Add detectors to analyser.
void TypeAnalysis::AddDetector(ITypeDetector *detector)
{
	this->detectors.push_back(detector);
}

std::string static GetRealName(const string& origin) {
	return origin;
}

#ifdef DEPENDENCYANALYSIS_TEST
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

	auto result = analyser.Analysis("../test_files/test.h");
	for (size_t i = 0; i < result.Size(); i++) {
		cout  << result[i].type << " " << result[i].name << " " << result[i].nameSpace << " " << result[i].wholeName <<endl;
	}
}
#endif