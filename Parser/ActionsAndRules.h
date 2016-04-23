/*
* MetricsExecutive.h: implementation of action and rules
* version: 1.0
* Languange: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
* Source: Jim Fawcett
*/
// Includes all rules and actions for analyse the c++ source code.
// If there is some new rules and actions, you should add the them to ConfigureParser.h

#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H

#include <queue>
#include <string>
#include <sstream>
#include <memory>
#include "Parser.h"
#include "../SemiExpression/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExpression/SemiExpression.h"
#include "../AstTree/ASTNode.h"
#include "../MetricsAnalysis/AstAnalyser.h"


 ///////////////////////////////////////////////////////////////
 // Repository instance is used to share resources
 // among all actions.
 /*
  * ToDo:
  * - add AST Node class
  * - provide field to hold root of AST
  * - provide interface to access AST
  * - provide helper functions to build AST, perhaps in Tree class
  */
class Repository  // application specific
{
private:
	ScopeStack<ASTNode*> stack;
	Scanner::Toker* p_Toker;
	ASTNode* root;
public:
	ASTNode*& getRoot();
	// Analyze the AST tree and print it out to console
	void showAST() {
		//AstAnalyser::analysis(root);
		AstAnalyser::printTree(root);
	}
	
	//Constructor
	Repository(Scanner::Toker* pToker)
	{
		p_Toker = pToker;
		root = new ASTNode();
		root->startLineCount = lineCount();
		root->name = "Global";
		root->type = "namespace";
	}
	
	//get scope stack
	ScopeStack<ASTNode*>& scopeStack()
	{
		return stack;
	}

	//get tokenizer of repository
	Scanner::Toker* Toker()
	{
		return p_Toker;
	}

	//get current line count
	size_t lineCount()
	{
		return (size_t)(p_Toker->getCurrentLineNumber());
	}

	//push a node to current stack
	//will also build the ast tree
	void push(ASTNode* node) {
		addAstNode(node);
		stack.push(node);
	}
	
	//return the current node of ast tree
	ASTNode* current() {
		ASTNode* node;
		if (stack.size() == 0) {
			node = root;
		}
		else {
			node = stack.top();
		}
		return node;
	}

	//pop out the element in the top of scope stack
	ASTNode* pop() {
		auto node = stack.pop();
		if (stack.size() == 0) {
			root->endLineCount = node->endLineCount;
		}
		else {
			auto elem = stack.top();
			elem->endLineCount = node->endLineCount;
		}
		return node;
	}

	//build ast node without changing the scope stack
	void addAstNode(ASTNode* node) {
		if (node->type != "unknown") {
			ASTNode* parent;
			if (stack.size() == 0) {
				parent = root;
			}
			else {
				parent = stack.top();
			}
			parent->children.push_back(node);
			node->parent = parent;
		}
	}
};


///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope
class BeginningOfScope : public IRule
{
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i < 5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}

	bool isNormalScope(ITokCollection*& pTc) {
		return pTc->find("{") < pTc->length() && (*pTc)[pTc->length() - 1].tokenValue == "{" && (*pTc)[pTc->find("{")].tokenType == TOKEN_TYPES::PUNCTUATOR;
	}
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;

		//Normal scope starts with "{"
		if (isNormalScope(pTc))
		{
			doActions(pTc);
			return true;
		}

		return true;
	}
};


///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope
class HandlePush : public IAction
{
	Repository* p_Repos;
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//push a anonymous node to the stack
		//the anonymous will be pop out by another handler later
		ASTNode* elem = new ASTNode();
		elem->type = "unknown";
		elem->name = "anonymous";
		elem->startLineCount = p_Repos->lineCount();
		p_Repos->push(elem);
	}
};


///////////////////////////////////////////////////////////////
// rule to detect end of scope
class EndOfScope : public IRule
{
public:
	//Test if the token collection is ended with closed bracket 
	static bool isClosedBracket(ITokCollection*& pTc) {
		return pTc->find("}") < pTc->length() && (*pTc)[pTc->length() - 1].tokenValue == "}" && (*pTc)[pTc->find("}")].tokenType == TOKEN_TYPES::PUNCTUATOR;
	}

	bool doTest(ITokCollection*& pTc)
	{
		if (EndOfScope::isClosedBracket(pTc))
		{
			doActions(pTc);
			return true;
		}
		/*
		else if ((*pTc)[pTc->length() - 1].tokenValue == ";" )
		{
			doActions(pTc);
			return true;
		}
		*/
		return true;
	}
};


///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope
class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}

	void doAction(ITokCollection*& pTc)
	{
		if (p_Repos->scopeStack().size() == 0)
			return;

		auto elem = p_Repos->scopeStack().top();
		if (EndOfScope::isClosedBracket(pTc))
		{
			elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;
			p_Repos->pop();
			return;
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements
class PreprocStatement : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("#") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};


///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console
class PrintPreproc : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
	}
};


///////////////////////////////////////////////////////////////
// rule to detect if statement
// Now it will only detect the if with bracket and ignore the 
// case that the if statement followed by sentence directly
class IfStatement : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i < 5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool isIfStatement(ITokCollection*& pTc) {
		auto ifIndex = pTc->find("if");
		auto elseIndex = pTc->find("else");
		auto isIf = ifIndex < pTc->length() && (*pTc)[ifIndex + 1].tokenValue == "(";
		auto isElseIf = ifIndex == elseIndex + 1;
		auto isElse = pTc->find("else") < pTc->length() && ifIndex == pTc->length();
		return isIf || isElseIf || isElse;
	}
	bool doTest(ITokCollection*& pTc)
	{
		if (isIfStatement(pTc)) {
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push if statement onto ScopeStack
class PushIf : public IAction
{
	Repository* p_Repos;
public:
	bool isIf(ITokCollection*& pTc) {
		return pTc->find("if") < pTc->length() && pTc->find("else") == pTc->length();
	}
	bool isElseIf(ITokCollection*& pTc) {
		return pTc->find("if") < pTc->length() && pTc->find("else") < pTc->length();
	}
	bool isElse(ITokCollection*& pTc) {
		return pTc->find("else") < pTc->length();
	}
	PushIf(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		auto token = (*pTc)[pTc->find("(") - 1];
		// push if scope
		ASTNode* elem = new ASTNode();
		std::string name;
		if (isIf(pTc))
			name = "if";
		else if (isElseIf(pTc))
			name = "else if";
		else
			name = "else";
		elem->startWith = "{";
		elem->type = "if";
		elem->name = name;
		elem->startLineCount = (*pTc)[0].lineNumber;
		elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;
		if ((*pTc)[pTc->length() - 1].tokenValue == "{") {
			// pop anonymous scope
			if (p_Repos->scopeStack().size() != 0 && p_Repos->scopeStack().top()->type == "unknown")
				p_Repos->scopeStack().pop();
			for (size_t i = 0; i < pTc->length(); i++) {
				//elem->tokens.push_back(pTc[i]);
			}
			p_Repos->push(elem);
		}
		else {
			//Currently it ignore the if statement which is without bracket scope
			//p_Repos->addAstNode(elem);
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect if statement
class ForStatement : public IRule
{
public:
	bool isFor(ITokCollection*& pTc) {
		auto isFor = pTc->find("for") < pTc->find("(") && pTc->find("(") < pTc->find(")") && pTc->find(")") < pTc->length();
		return isFor;
	}
	bool doTest(ITokCollection*& pTc)
	{
		if (isFor(pTc)) {
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push for statement onto ScopeStack

class PushFor : public IAction
{
	Repository* p_Repos;
public:
	bool isFor(ITokCollection*& pTc) {
		auto isFor = pTc->find("for") < pTc->find("(") && pTc->find("(") < pTc->find(")") && pTc->find(")") < pTc->length();
		return isFor;
	}
	PushFor(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		auto token = (*pTc)[pTc->find("(") - 1];
		// push function scope
		ASTNode* elem = new ASTNode();
		std::string name = "for";
		elem->type = "for";
		elem->name = name;
		elem->startLineCount = (*pTc)[0].lineNumber;
		elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;
		if ((*pTc)[pTc->length() - 1].tokenValue == "{") {
			elem->startWith = "{";
			// pop anonymous scope
			if (p_Repos->scopeStack().size() != 0 && p_Repos->scopeStack().top()->type == "unknown")
				p_Repos->scopeStack().pop();
			for (size_t i = 0; i < pTc->length(); i++) {
				//elem->tokens.push_back((*pTc[i]);
			}
			p_Repos->push(elem);
		}
		else {
			//Currently it ignore the for statement without bracket scope
			//p_Repos->addAstNode(elem);
		}
		
	}
};

///////////////////////////////////////////////////////////////
// rule to detect switch statement
class SwitchStatement : public IRule
{
public:
	bool isSwitchScope(ITokCollection*& pTc) {
		auto isSwitch = pTc->find("switch") < pTc->find("(") && pTc->find("(") < pTc->find(")") && pTc->find(")") < pTc->length();
		auto isCase = pTc->find("case") < pTc->find(":") && pTc->find(":") < pTc->length();
		auto isDefault = pTc->find("default") < pTc->find(":") && pTc->find(":") < pTc->length();
		return isSwitch || isCase || isDefault;
	}

	bool doTest(ITokCollection*& pTc)
	{
		if (isSwitchScope(pTc)) {
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push switch statement onto ScopeStack
class PushSwitch : public IAction
{
	Repository* p_Repos;
public:
	bool isSwitch(ITokCollection*& pTc) {
		auto isSwitch = pTc->find("switch") < pTc->find("(") && pTc->find("(") < pTc->find(")") && pTc->find(")") < pTc->length();
		return isSwitch;
	}
	bool isCase(ITokCollection*& pTc) {
		auto isCase = pTc->find("case") < pTc->find(":") && pTc->find(":") < pTc->length();
		return isCase;
	}
	bool isDefault(ITokCollection*& pTc) {
		auto isDefault = pTc->find("default") < pTc->find(":") && pTc->find(":") < pTc->length();
		return isDefault;
	}
	PushSwitch(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		auto token = (*pTc)[pTc->find("(") - 1];
		// push function scope
		ASTNode* elem = new ASTNode();
		std::string name;
		if (isSwitch(pTc))
			name = "switch";
		else if (isCase(pTc))
			name = "case";
		else
			name = "default";
		elem->type = "switch";
		elem->name = name;
		elem->startLineCount = (*pTc)[0].lineNumber;
		elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;
		if ((*pTc)[pTc->length() - 1].tokenValue == "{") {
			elem->startWith = "{";
			// pop anonymous scope
			if (p_Repos->scopeStack().size() != 0 && p_Repos->scopeStack().top()->type == "unknown")
				p_Repos->scopeStack().pop();
			for (size_t i = 0; i < pTc->length(); i++) {
				//elem->tokens.push_back(&(*pTc)[i]);
			}
			p_Repos->push(elem);
		}
		else {
			//Currently it ignore the switch statement which is without bracket scope
			//p_Repos->addAstNode(elem);
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect switch statement
class TryStatement : public IRule
{
public:
	bool isTry(ITokCollection*& pTc) {
		auto isTry = pTc->find("try") < pTc->find("{") && pTc->find("{") < pTc->length();
		auto isCatch = pTc->find("catch") < pTc->find("(") && pTc->find("(") < pTc->find(")") && pTc->find(")") < pTc->length();
		return isTry || isCatch;
	}

	bool doTest(ITokCollection*& pTc)
	{
		if (isTry(pTc)) {
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push switch statement onto ScopeStack
class PushTry : public IAction
{
	Repository* p_Repos;
public:
	bool isTry(ITokCollection*& pTc) {
		auto isTry = pTc->find("try") < pTc->find("{") && pTc->find("{") < pTc->length();
		return isTry;
	}

	bool isCatch(ITokCollection*& pTc) {
		auto isCatch = pTc->find("catch") < pTc->find("(") && pTc->find("(") < pTc->find(")") && pTc->find(")") < pTc->length();
		return isCatch;
	}
	PushTry(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		auto token = (*pTc)[pTc->find("(") - 1];
		// push function scope
		ASTNode* elem = new ASTNode();
		std::string name;
		if (isTry(pTc))
			name = "try";
		else if (isCatch(pTc))
			name = "case";

		elem->type = "try";
		elem->name = name;
		elem->startLineCount = (*pTc)[0].lineNumber;
		elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;
		if ((*pTc)[pTc->length() - 1].tokenValue == "{") {
			elem->startWith = "{";
			// pop anonymous scope
			if (p_Repos->scopeStack().size() != 0 && p_Repos->scopeStack().top()->type == "unknown")
				p_Repos->scopeStack().pop();
			for (size_t i = 0; i < pTc->length(); i++) {
				//elem->tokens.push_back(&(*pTc)[i]);
			}
			p_Repos->push(elem);
		}
		else {
			//Currently it ignore the try statement which is without bracket scope
			//p_Repos->addAstNode(elem);
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect if statement
class WhileStatement : public IRule
{
public:
	bool isWhile(ITokCollection*& pTc) {
		auto isDo = pTc->find("do") < pTc->find("{") && pTc->find("{") < pTc->length();
		auto isWhile = pTc->find("while") < pTc->find("(") && pTc->find("(") < pTc->find(")") && pTc->find(")") < pTc->length();
		return isWhile || isDo;
	}
	bool doTest(ITokCollection*& pTc)
	{
		if (isWhile(pTc)) {
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push for statement onto ScopeStack

class PushWhile: public IAction
{
	Repository* p_Repos;
public:
	bool isWhile(ITokCollection*& pTc) {
		auto isWhile = pTc->find("while") < pTc->find("(") && pTc->find("(") < pTc->find(")") && pTc->find(")") < pTc->length();
		return isWhile;
	}

	bool isDo(ITokCollection*& pTc) {
		auto isDo = pTc->find("do") < pTc->find("{") && pTc->find("{") < pTc->length();
		return isDo;
	}
	PushWhile(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		
		auto token = (*pTc)[pTc->find("(") - 1];
		// push function scope
		ASTNode* elem = new ASTNode();
		std::string name = "while";

		if (isDo(pTc))
			name = "do";

		elem->type = "while";
		elem->name = name;
		elem->startLineCount = (*pTc)[0].lineNumber;
		elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;
		if ((*pTc)[pTc->length() - 1].tokenValue == "{") {
			elem->startWith = "{";
			// pop anonymous scope
			if (p_Repos->scopeStack().size() != 0 && p_Repos->scopeStack().top()->type == "unknown")
				p_Repos->scopeStack().pop();
			for (size_t i = 0; i < pTc->length(); i++) {
				//elem->tokens.push_back(&(*pTc)[i]);
			}
			p_Repos->push(elem);
		}
		else {
			//Currently it ignore the while statement which is without bracket scope
			//p_Repos->addAstNode(elem);
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect class or structure statement
class ClassOrStructDeclaration : public IRule
{
public:
	bool isClassOrStruct(ITokCollection*& pTc) {
		auto isClass = pTc->find("class") < pTc->find("{") && pTc->find("{") < pTc->length();
		auto isStructure = pTc->find("struct") < pTc->find("{") && pTc->find("{") < pTc->length();
		auto isEnum = pTc->find("enum") < pTc->find("{") && pTc->find("{") < pTc->length();
		return isStructure || isClass || isEnum;
	}

	bool doTest(ITokCollection*& pTc)
	{
		if (isClassOrStruct(pTc)) {
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push for statement onto ScopeStack

class PushClassOrStruct : public IAction
{
	Repository* p_Repos;
public:
	bool isClass(ITokCollection*& pTc) {
		auto isClass = pTc->find("class") < pTc->find("{") && pTc->find("{") < pTc->length();
		return isClass;
	}
	bool isStruct(ITokCollection*& pTc) {
		auto isStructure = pTc->find("struct") < pTc->find("{") && pTc->find("{") < pTc->length();
		return isStructure;
	}
	bool isEnum(ITokCollection*& pTc) {
		auto isStructure = pTc->find("enum") < pTc->find("{") && pTc->find("{") < pTc->length();
		return isStructure;
	}
	PushClassOrStruct(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		auto token = (*pTc)[pTc->find("(") - 1];
		// push function scope
		ASTNode* elem = new ASTNode();
		if (isClass(pTc))
			elem->type = "class";
		else if(isStruct(pTc))
			elem->type = "struct";
		else if (isEnum(pTc))
			elem->type = "enum";
		elem->name = (*pTc)[pTc->find(elem->type) + 1].tokenValue;
		elem->startLineCount = (*pTc)[0].lineNumber;
		elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;
		for (size_t i = 0; i < pTc->length(); i++) {
			//elem->tokens.push_back(&(*pTc)[i]);
		}
		if ((*pTc)[pTc->length() - 1].tokenValue == "{") {
			elem->startWith = "{";
			// pop anonymous scope
			if (p_Repos->scopeStack().size() != 0 && p_Repos->scopeStack().top()->type == "unknown")
				p_Repos->scopeStack().pop();
			for (size_t i = 0; i < pTc->length(); i++) {
				elem->tokens.push_back((*pTc)[i]);
			}
			p_Repos->push(elem);
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect namespace statement
class NamespaceDeclaration : public IRule
{
public:
	bool isNamespace(ITokCollection*& pTc) {
		auto isNamespace = pTc->find("namespace") < pTc->find("{") && pTc->find("{") < pTc->length();
		return isNamespace;
	}

	bool doTest(ITokCollection*& pTc)
	{
		if (isNamespace(pTc)) {
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push namepsace statement onto ScopeStack
class PushNamespace : public IAction
{
	Repository* p_Repos;
public:
	bool isNamespace(ITokCollection*& pTc) {
		auto isNamespace = pTc->find("namespace") < pTc->find("{") && pTc->find("{") < pTc->length();
		return isNamespace;
	}
	PushNamespace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		auto token = (*pTc)[pTc->find("(") - 1];
		// push function scope
		ASTNode* elem = new ASTNode();
		elem->type = "namespace";

		elem->name = (*pTc)[pTc->find(elem->type) + 1].tokenValue;
		elem->startLineCount = (*pTc)[0].lineNumber;
		elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;

		if ((*pTc)[pTc->length() - 1].tokenValue == "{") {
			elem->startWith = "{";
			// pop anonymous scope
			if (p_Repos->scopeStack().size() != 0 && p_Repos->scopeStack().top()->type == "unknown")
				p_Repos->scopeStack().pop();
			for (size_t i = 0; i < pTc->length(); i++) {
				//elem->tokens.push_back(&(*pTc)[i]);
			}
			p_Repos->push(elem);
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect class or structure statement
class SimpleScopeStatement : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->length() == 1 && pTc->find("{") == 0) {
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push for statement onto ScopeStack

class PushSimpleScope : public IAction
{
	Repository* p_Repos;
public:
	PushSimpleScope(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		auto token = (*pTc)[pTc->find("(") - 1];
		// push function scope
		ASTNode* elem = new ASTNode();
		elem->type = "simple scope";
		elem->name = "{";
		elem->startLineCount = (*pTc)[0].lineNumber;
		elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;

		if ((*pTc)[pTc->length() - 1].tokenValue == "{") {
			elem->startWith = "{";
			// pop anonymous scope
			if (p_Repos->scopeStack().size() != 0 && p_Repos->scopeStack().top()->type == "unknown")
				p_Repos->scopeStack().pop();
			for (size_t i = 0; i < pTc->length(); i++) {
				//elem->tokens.push_back(&(*pTc)[i]);
			}
			p_Repos->push(elem);
		}
	}
};


///////////////////////////////////////////////////////////////
// rule to detect function definitions
class FunctionDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i < 5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool isLambda(ITokCollection*& pTc) {
		return  pTc->find("{") < pTc->length() && (*pTc)[pTc->find("{") - 1].tokenValue == "]";
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1].tokenValue == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && (!isSpecialKeyWord(tc[len - 1].tokenValue) || isLambda(pTc) ))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack
class PushFunction : public IAction
{
	Repository* p_Repos;
public:
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		if (p_Repos->scopeStack().size() != 0 && p_Repos->scopeStack().top()->type == "unknown")
			p_Repos->scopeStack().pop();
		size_t startIndex = 0;

		auto endIndex = pTc->find("{");
		if (startIndex >= pTc->length()) {
			startIndex = endIndex - 1;
		}
		std::string name;
		for (size_t i = startIndex ; i < endIndex; i++) {
			name += (*pTc)[i].tokenValue + " ";
		}
		auto token = (*pTc)[pTc->find("(") - 1];
		// push function scope
		ASTNode* elem = new ASTNode();
		elem->type = "function";
		elem->name = name;
		elem->startLineCount = (*pTc)[0].lineNumber;
		elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;
		elem->startWith = "{";

		for (size_t i = 0; i < pTc->length(); i++) {
			elem->tokens.push_back((*pTc)[i]);
		}

		p_Repos->push(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
	Repository* p_Repos;
public:
	PrintFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n  FuncDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		pTc->remove("public");
		pTc->remove(":");
		pTc->trimFront();
		int len = pTc->find(")");
		/*std::cout << "\n\n  Pretty Stmt:    ";
		for (int i = 0; i < len + 1; ++i)
			std::cout << (*pTc)[i].tokenValue << " ";
		std::cout << "\n";*/
	}
};

///////////////////////////////////////////////////////////////
// rule to detect declaration

class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
	bool isModifier(const std::string& tok) // modifiers and initializers.
	{                                       // If you have two things left
		const size_t numKeys = 22;            // its declar else executable.
		const static std::string keys[numKeys] = {
		  "const", "extern", "friend", "mutable", "signed", "static",
		  "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
		  "public", "protected", "private", ":", "typename", "typedef", "++", "--"
		};
		for (int i = 0; i < numKeys; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void condenseTemplateTypes(ITokCollection& tc)
	{
		size_t start = tc.find("<");
		size_t end = tc.find(">");
		if (start >= end || start == 0)
			return;
		else
		{
			if (end == tc.length())
				end = tc.find(">::");
			if (end == tc.length())
				return;
		}
		std::string save = tc[end].tokenValue;
		std::string tok = tc[start - 1].tokenValue;
		for (size_t i = start; i < end + 1; ++i)
			tok += tc[i].tokenValue;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		if (save == ">::")
		{
			tok += tc[start + 1].tokenValue;
			tc.remove(start);
		}
		tc[start - 1].tokenValue = tok;
		//std::cout << "\n  -- " << tc.show();
	}
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i < 5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void removeInvocationParens(ITokCollection& tc)
	{
		size_t start = tc.find("(");
		size_t end = tc.find(")");
		if (start >= end || end == tc.length() || start == 0)
			return;
		if (isSpecialKeyWord(tc[start - 1].tokenValue))
			return;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		//std::cout << "\n  -- " << tc.show();
	}
	bool doTest(ITokCollection*& pTc)
	{
		Scanner::ITokCollection& in = *pTc;
		Scanner::SemiExpression tc;
		for (size_t i = 0; i < in.length(); ++i)
			tc.push_back(in[i]);

		if (tc[tc.length() - 1].tokenValue == ";" && tc.length() > 2)
		{
			removeInvocationParens(tc);
			condenseTemplateTypes(tc);

			Scanner::SemiExpression se;
			for (size_t i = 0; i < tc.length(); ++i)
			{
				if (isModifier(tc[i].tokenValue) || se.isComment(tc[i]) || tc[i].tokenValue == "\n" || tc[i].tokenValue == "return")
					continue;
				if (tc[i].tokenValue == "=" || tc[i].tokenValue == ";")
					break;
				else
					se.push_back(tc[i]);
			}

			if (se.length() == 2)  // type & name, so declaration
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push for statement onto ScopeStack

class PushDeclaration : public IAction
{
	Repository* p_Repos;
public:
	PushDeclaration(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		auto token = (*pTc)[pTc->find("(") - 1];
		// push function scope
		ASTNode* elem = new ASTNode();
		elem->type = "declaration";
		elem->name = pTc->toString();
		elem->startLineCount = (*pTc)[0].lineNumber;
		elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;
		for (size_t i = 0; i < pTc->length(); i++) {
			elem->tokens.push_back((*pTc)[i]);
		}
		p_Repos->addAstNode(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to show declaration

class ShowDeclaration : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		//ITokCollection& tc = *pTc;
		//// remove comments
		//Scanner::SemiExpression se;
		//for (size_t i = 0; i < tc.length(); ++i)
		//	if (!se.isComment(tc[i]))
		//		se.push_back(tc[i]);
		//// show cleaned semiExp
		//std::cout << "\n  Declaration: " << se.show();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect executable

class Executable : public IRule           // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
	bool isModifier(const std::string& tok) // modifiers and initializers.
	{                                       // If you have two things left
		const size_t numKeys = 22;            // its declar else executable.
		const static std::string keys[numKeys] = {
		  "const", "extern", "friend", "mutable", "signed", "static",
		  "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
		  "public", "protected", "private", ":", "typename", "typedef", "++", "--"
		};
		for (int i = 0; i < numKeys; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void condenseTemplateTypes(ITokCollection& tc)
	{
		size_t start = tc.find("<");
		size_t end = tc.find(">");
		if (start >= end || start == 0)
			return;
		else
		{
			if (end == tc.length())
				end = tc.find(">::");
			if (end == tc.length())
				return;
		}
		std::string save = tc[end].tokenValue;
		std::string tok = tc[start - 1].tokenValue;
		for (size_t i = start; i < end + 1; ++i)
			tok += tc[i].tokenValue;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		if (save == ">::")
		{
			tok += tc[start + 1].tokenValue;
			tc.remove(start);
		}
		tc[start - 1].tokenValue = tok;
	}

	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i < 5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void removeInvocationParens(ITokCollection& tc)
	{
		size_t start = tc.find("(");
		size_t end = tc.find(")");
		if (start >= end || end == tc.length() || start == 0)
			return;
		if (isSpecialKeyWord(tc[start - 1].tokenValue))
			return;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& in = *pTc;
		Scanner::SemiExpression tc;
		for (size_t i = 0; i < in.length(); ++i)
			tc.push_back(in[i]);

		if (tc[tc.length() - 1].tokenValue == ";" && tc.length() > 2)
		{
			removeInvocationParens(tc);
			condenseTemplateTypes(tc);

			// remove modifiers, comments, newlines, returns, and initializers

			Scanner::SemiExpression se;
			for (size_t i = 0; i < tc.length(); ++i)
			{
				if (isModifier(tc[i].tokenValue) || se.isComment(tc[i]) || tc[i].tokenValue == "\n" || tc[i].tokenValue == "return")
					continue;
				if (tc[i].tokenValue == "=" || tc[i].tokenValue == ";")
					break;
				else
					se.push_back(tc[i]);
			}

			if (se.length() != 2)  // not a declaration
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push for statement onto ScopeStack

class PushExecutable : public IAction
{
	Repository* p_Repos;
public:
	PushExecutable(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		auto token = (*pTc)[pTc->find("(") - 1];
		// push function scope
		ASTNode* elem = new ASTNode();
		elem->type = "executable";
		elem->name = pTc->toString();
		elem->startLineCount = (*pTc)[0].lineNumber;
		elem->endLineCount = (*pTc)[pTc->length() - 1].lineNumber;
		for (size_t i = 0; i < pTc->length(); i++) {
			elem->tokens.push_back((*pTc)[i]);
		}
		p_Repos->addAstNode(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to show executable

class ShowExecutable : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		//ITokCollection& tc = *pTc;
		//// remove comments
		//Scanner::SemiExpression se;
		//for (size_t i = 0; i < tc.length(); ++i)
		//{
		//	if (!se.isComment(tc[i]))
		//		se.push_back(tc[i]);
		//}
		//// show cleaned semiExp
		//std::cout << "\n  Executable: " << se.show();
	}
};

#endif
