/*
* AnalysisHelper.h
* Includes some helper codes for analysising dependency and type definition
*
* Author: Junkai Yang
*/
#include "../AstTree/ASTNode.h"


class AnalysisHelper {
public:
	// This method can get an ast node's namespace
	static string getNamespace(ASTNode* node);
};