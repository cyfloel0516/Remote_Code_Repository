#include "TypeDetector.h"
#include "AnalysisHelper.h"

// class definition detector
bool ClassDefinition::test(ASTNode*& node)
{
	if (node->type == "class" || node->type == "enum" || node->type == "struct")
	{
		return true;
	}
	return false;
}

TypeDefinition ClassDefinition::getData(ASTNode*& node)
{
	// To get the real name of class
	auto tokens = utils::split(node->getFullName(), ' ');
	auto splitIndex = 0;
	auto colonIndex = std::find(tokens.begin(), tokens.end(), ":");
	if (colonIndex != tokens.end())
		splitIndex = colonIndex - tokens.begin();
	else
		splitIndex = tokens.size() - 1;
	string realName = tokens[splitIndex - 1];
	auto type = TypeDefinition(realName, node->type, "", "", node->getFullName());
	return type;
}


// typedef declaration detector
bool TypeDefDefinition::test(ASTNode *& node)
{

	string wholeName = node->getFullName();
	auto tokens = utils::split(wholeName, ' ');

	if (node->type == "declaration" 
		&& std::find(tokens.begin(), tokens.end(), "typedef") == tokens.begin()) {
		return true;
	}

	return false;
}

TypeDefinition TypeDefDefinition::getData(ASTNode *& node)
{
	// To get the real name of typedef
	auto wholeName = node->getFullName();
	auto tokens = utils::split(wholeName, ' ');
	auto realName = tokens[tokens.size() - 2];
	auto type = TypeDefinition(realName, "typedef", "", "", node->getFullName());
	return type;
}

bool AliasDefinition::test(ASTNode *& node)
{
	string wholeName = node->getFullName();
	auto tokens = utils::split(wholeName, ' ');
	if (node->type == "declaration" 
		&& std::find(tokens.begin(), tokens.end(), "using") != tokens.end() 
		&& node->name.find("=") != string::npos) {
		return true;
	}
	return false;
}

TypeDefinition AliasDefinition::getData(ASTNode *& node)
{
	auto wholeName = node->getFullName();
	auto tokens = utils::split(wholeName, ' ');
	auto equalIndex = std::find(tokens.begin(), tokens.end(), "=");
	
	auto realName = *(equalIndex - 1);

	auto type = TypeDefinition(realName, "using" , "", "", node->getFullName());
	return type;
}

bool GlobalFunctionDefinition::isLambda(ASTNode *& node)
{
	auto wholeName = node->getFullName();
	auto tokens = utils::split(wholeName, ' ');
	if (std::find(tokens.begin(), tokens.end(), "function") != tokens.end()) {
		return true;
	}
	return false;
}

bool GlobalFunctionDefinition::test(ASTNode *& node)
{
	string fullName = node->getFullName();
	if (node->type == "function" &&node->parent != nullptr 
		&& node->parent->type == "namespace" && fullName.find("::") == string::npos ) {
		return true;
	}
	else if (isLambda(node)) {
		return true;
	}
	return false;
}

TypeDefinition GlobalFunctionDefinition::getData(ASTNode *& node)
{
	string realName;
	auto wholeName = node->getFullName();
	auto tokens = utils::split(wholeName, ' ');
	if (isLambda(node)) {
		auto equalIndex = std::find(tokens.begin(), tokens.end(), "=");
		realName = *(equalIndex - 1);
	}
	else {
		auto parenIndex = std::find(tokens.begin(), tokens.end(), "(");
		realName = *(parenIndex - 1);
	}
	auto type = TypeDefinition(realName, node->type, "", "", node->getFullName());
	return type;
}
