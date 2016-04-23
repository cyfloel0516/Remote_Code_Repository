/*
* DependencyAnalysis.h
* This file is used for analysising the dependency relation between files.
* Analysis uses typetable to detect dependency and return a dependency table as result.
*
* Author: Junkai Yang
*/
#ifndef DEPENDENCYANALYSIS_H
#define DEPENDENCYANALYSIS_H

#include "TypeTable.h"
#include "DependencyTable.h"
#include "DependencyDetector.h"
#include <vector>

using namespace std;

class DependencyAnalysis {
private:
	TypeTable* _pTypeTable;
	vector<IDependencyDetector*> detectors;
public:
	// Constructor
	DependencyAnalysis();
	DependencyAnalysis(TypeTable* table);
	
	// Set the context(type definition table) for analysising
	void SetTypeTable(TypeTable* table);
	
	// Execute analysis
	DependencyTable Analysis(std::string fileName);

	// Add detector for analysis
	void AddDetector(IDependencyDetector* detector);
	~DependencyAnalysis();
};

#endif