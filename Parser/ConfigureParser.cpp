/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
//  ver 2.1                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Author:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

// Edited by Junkai Yang, add some rules and actions.

#include <fstream>
#include "Parser.h"
#include "../SemiExpression/SemiExpression.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------
ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  delete pIfStatement;
  delete pPushIf;
  delete pSwitchStatement;
  delete pPushSwitch;
  delete pForStatement;
  delete pPushFor;
  delete pWhileStatement;
  delete pPushWhile;
  delete pPushDeclaration;
  delete pPushExecutable;
  
  delete pClassOrStructDeclaration;
  delete pPushClassOrStruct;

  delete pNamespaceDeclaration;
  delete pPushNamespace;

  delete pSimpleScopeStatement;
  delete pPushSimpleScope;

  pIn->close();
  delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build()
{
  try
  {
    // add Parser's main parts

    pToker = new Toker;
    pToker->returnComments(false);
    pSemi = new SemiExpression(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);
    // configure to manage scope
    // these must come first - they return true on match
    // so rule checking continues

	this->BuildBeginScope();
	this->BuildEndScope();
	this->BuildOthers();



    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}
void ConfigParseToConsole::BuildBeginScope()
{
	// 1. Begin scope detection
	pBeginningOfScope = new BeginningOfScope();
	pHandlePush = new HandlePush(pRepo);
	pBeginningOfScope->addAction(pHandlePush);pParser->addRule(pBeginningOfScope);

	// 2. if, for, while, switch, namespace, function, class, struct
	pIfStatement = new IfStatement;
	pPushIf = new PushIf(pRepo);
	pIfStatement->addAction(pPushIf);pParser->addRule(pIfStatement);

	pForStatement = new ForStatement;
	pPushFor = new PushFor(pRepo);
	pForStatement->addAction(pPushFor);pParser->addRule(pForStatement);

	pSwitchStatement = new SwitchStatement;
	pPushSwitch = new PushSwitch(pRepo);
	pSwitchStatement->addAction(pPushSwitch);pParser->addRule(pSwitchStatement);

	pWhileStatement = new WhileStatement;
	pPushWhile = new PushWhile(pRepo);
	pWhileStatement->addAction(pPushWhile);pParser->addRule(pWhileStatement);

	pFunctionDefinition = new FunctionDefinition;
	pPushFunction = new PushFunction(pRepo);  // no action
	pPrintFunction = new PrintFunction(pRepo);
	pFunctionDefinition->addAction(pPushFunction);
	pFunctionDefinition->addAction(pPrintFunction);
	pParser->addRule(pFunctionDefinition);

	pClassOrStructDeclaration = new ClassOrStructDeclaration;
	pPushClassOrStruct = new PushClassOrStruct(pRepo);
	pClassOrStructDeclaration->addAction(pPushClassOrStruct);pParser->addRule(pClassOrStructDeclaration);

	pNamespaceDeclaration = new NamespaceDeclaration;
	pPushNamespace = new PushNamespace(pRepo);
	pNamespaceDeclaration->addAction(pPushNamespace);pParser->addRule(pNamespaceDeclaration);

	pTryStatement = new TryStatement;
	pPushTry = new PushTry(pRepo);
	pTryStatement->addAction(pPushTry);pParser->addRule(pTryStatement);
}
void ConfigParseToConsole::BuildEndScope()
{
	// 3. Other scope cases
	pSimpleScopeStatement = new SimpleScopeStatement();
	pPushSimpleScope = new PushSimpleScope(pRepo);
	pSimpleScopeStatement->addAction(pPushSimpleScope);
	pParser->addRule(pSimpleScopeStatement);

	// 4. End Scope
	pEndOfScope = new EndOfScope();
	pHandlePop = new HandlePop(pRepo);
	pEndOfScope->addAction(pHandlePop);
	pParser->addRule(pEndOfScope);

}
void ConfigParseToConsole::BuildOthers()
{
	// 5. Others
	// configure to detect and act on declarations and Executables
	pDeclaration = new Declaration;
	pShowDeclaration = new ShowDeclaration;
	pPushDeclaration = new PushDeclaration(pRepo);
	pDeclaration->addAction(pShowDeclaration);
	pDeclaration->addAction(pPushDeclaration);

	pParser->addRule(pDeclaration);
	pExecutable = new Executable;
	pShowExecutable = new ShowExecutable;
	pPushExecutable = new PushExecutable(pRepo);
	pExecutable->addAction(pShowExecutable);
	pExecutable->addAction(pPushExecutable);
	pParser->addRule(pExecutable);
}
//----< test stub >--------------------------------------------
#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
