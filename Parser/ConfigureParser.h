#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
/////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers              //
//  ver 2.1                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Author:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
// Edited by Junkai Yang, add some rules and actions.              //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  This module builds and configures parsers.  It builds the parser
  parts and configures them with application specific rules and actions.

  Public Interface:
  =================
  ConfigParseToConsole conConfig;
  conConfig.Build();
  conConfig.Attach(someFileName);

  Build Process:
  ==============
  Required files
	- ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
	  ActionsAndRules.h, ActionsAndRules.cpp,
	  SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
	- devenv Project1HelpS06.sln
	- cl /EHsc /DTEST_PARSER ConfigureParser.cpp parser.cpp \
		 ActionsAndRules.cpp \
		 semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.1 : 19 Feb 16
  - Added PrintFunction action to FunctionDefinition rule
  ver 2.0 : 01 Jun 11
  - Major revisions to begin building a strong code analyzer
  ver 1.1 : 01 Feb 06
  - cosmetic changes to ConfigureParser.cpp
  ver 1.0 : 12 Jan 06
  - first release

*/

//
#include <fstream>
#include "Parser.h"
#include "../SemiExpression/SemiExpression.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"

///////////////////////////////////////////////////////////////
// build parser that writes its output to console


class Parser;

class ConfigParseToConsole : IBuilder
{
public:
	ConfigParseToConsole() {};
	~ConfigParseToConsole();
	bool Attach(const std::string& name, bool isFile = true);
	Parser* Build();
	void BuildBeginScope();
	void BuildEndScope();
	void BuildOthers();
	Repository* pRepo;
private:
	// Builder must hold onto all the pieces

	std::ifstream* pIn;
	Scanner::Toker* pToker;
	Scanner::SemiExpression* pSemi;
	Parser* pParser;

	// add Rules and Actions

	BeginningOfScope* pBeginningOfScope;
	HandlePush* pHandlePush;
	EndOfScope* pEndOfScope;
	HandlePop* pHandlePop;
	FunctionDefinition* pFunctionDefinition;
	PushFunction* pPushFunction;
	PrintFunction* pPrintFunction;
	Declaration* pDeclaration;
	ShowDeclaration* pShowDeclaration;
	Executable* pExecutable;
	ShowExecutable* pShowExecutable;
	IfStatement* pIfStatement;
	PushIf* pPushIf;

	ForStatement* pForStatement;
	PushFor* pPushFor;

	SwitchStatement* pSwitchStatement;
	PushSwitch* pPushSwitch;

	WhileStatement* pWhileStatement;
	PushWhile* pPushWhile;

	PushDeclaration* pPushDeclaration;
	PushExecutable* pPushExecutable;

	ClassOrStructDeclaration* pClassOrStructDeclaration;
	PushClassOrStruct* pPushClassOrStruct;

	NamespaceDeclaration* pNamespaceDeclaration;
	PushNamespace* pPushNamespace;

	TryStatement* pTryStatement;
	PushTry* pPushTry;

	SimpleScopeStatement* pSimpleScopeStatement;
	PushSimpleScope* pPushSimpleScope;
	// prohibit copies and assignments

	ConfigParseToConsole(const ConfigParseToConsole&) = delete;
	ConfigParseToConsole& operator=(const ConfigParseToConsole&) = delete;
};


#endif
