#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H
/*
* TypeAnalysis.h
* This package provides ability to find all types definition for a specified file 
* and return a type table as result.
*
* Author: Junkai Yang
*/

#include <string>
#include <tuple>
#include <vector>
#include "TypeDetector.h"
#include "TypeTable.h"
using namespace std;


// this class is to hold the result of tyoe analysis.
// Each type definition of analysis contains four elements: 
// <name, type, namespace, file_path>
//using TypeDefinition = typename std::tuple<string, string, string, string>;
class TypeAnalysis {
public:
	// Execute analysis
	TypeTable Analysis(std::string filePath);

	// Add detector for analysis
	void AddDetector(ITypeDetector* detector);
private:
	std::vector<ITypeDetector*> detectors;
};

#endif