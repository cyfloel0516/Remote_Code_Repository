/* 
* Tokenizer.cpp: Implementation of tokenizer
* version: 1.0
* Language: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
* Source: Jim Fawcett, Syracuse University, CST 4-187
*/

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "Utils.h"

using namespace Scanner;
using namespace utils;
//--begin--Implementation of 
Token::Token() {
	tokenValue = "";
	tokenType = "";
}

void Token::clear() {
	tokenValue = "";
	tokenType = "";
}
//--end--implementation of Token

//--begin--declaration of TOKEN_TYPES
const std::string TOKEN_TYPES::TERMINATED = "Terminated";
const std::string TOKEN_TYPES::STRING = "String";
const std::string TOKEN_TYPES::CPPCOMMENT = "CppComment";
const std::string TOKEN_TYPES::CCOMMENT = "CComment";
const std::string TOKEN_TYPES::NEWLINE = "Newline";
const std::string TOKEN_TYPES::SPACE = "Space";
const std::string TOKEN_TYPES::PUNCTUATOR = "Punctuator";
const std::string TOKEN_TYPES::ALPHANUM = "Alphanum";
//--end--declaration of TOKEN_TYPES

//--begin--delcaration of Consumer Context
class Scanner::Context {
public:
	Context();
	~Context();
	//functions
	bool isSpecialSingleChar(const char &c);
	bool isSpecialSingleChar(const std::string &c);
	bool isSpecialCharPair(const std::string &p);
	void addSingleToken(const std::string& token);
	void addDoubleToken(const std::string& token);
	void replaceSingleToken(const std::vector<std::string>& tokens);
	void replaceDoubleToken(const std::vector<std::string>& tokens);
	std::vector<std::string> getSingleTokens();
	std::vector<std::string> getDoubleTokens();

	//Properties
	Token token;
	std::istream* _pIn;
	int lineNumber;
	int tokenIndex;
	bool inDoubleQuotes;
	bool inSingleQuotes;
	int currChar;
	int prevChar;
	bool _returnComment = false;

	//Consumer State
	ConsumeState* _pState;
	ConsumeState* _pEatCppComment;
	ConsumeState* _pEatCComment;
	ConsumeState* _pEatWhitespace;
	ConsumeState* _pEatPunctuator;
	ConsumeState* _pEatAlphanum;
	ConsumeState* _pEatNewline;
	ConsumeState* _pEatString;
	ConsumeState* _pTerminate;
	std::vector<std::string> singleTokens;
	std::vector<std::string> doubleTokens;

	void clear() {
		currChar = char();
		token.clear();
		lineNumber = 1;
		tokenIndex = 1;
		_pIn = nullptr;
		_pState = _pEatWhitespace;
	}


};
/////////////////////////////////////////////////////////////////////
// Private abstract class ConsumeState
class Scanner::ConsumeState
{
public:
	ConsumeState();
	ConsumeState(Context* pContext);
	virtual ~ConsumeState();

	void attach(std::istream* pIn) { _pContext->_pIn = pIn; }

	virtual void eatChars() = 0;

	bool collectChar();
	void consumeChars() {
		_pContext->_pState->eatChars();
		_pContext->_pState->nextState();
	}

	bool canRead() { return _pContext->_pIn->good(); }
	Token getTok() { return _pContext->token; }
	bool hasTok() { return !_pContext->token.tokenType.empty(); }
	ConsumeState* nextState();

	void setContext(Context* pContext) {
		this->_pContext = pContext;
	}
protected:
	Context* _pContext;
};
bool Scanner::ConsumeState::collectChar() {
	if (_pContext->_pIn->good())
	{
		_pContext->prevChar = _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		if (_pContext->currChar == '\n')
		{ 
			++(_pContext->lineNumber);
			_pContext->tokenIndex = 0;
		}
		return true;
	}
	else {
		_pContext->prevChar = _pContext->currChar;
		_pContext->currChar = EOF;
		return false;
	}
}
//Get special single tokens;
std::vector<std::string> Context::getSingleTokens(){
	return std::vector<std::string>(singleTokens);
}

//Get special double tokens;
std::vector<std::string> Context::getDoubleTokens() {
	return std::vector<std::string>(doubleTokens);
}

//add a special single token;
void Context::addSingleToken(const std::string& token) {
	singleTokens.push_back(token);
}

//add a special double token;
void Context::addDoubleToken(const std::string& token) {
	doubleTokens.push_back(token);
}

//replace special single token;
void Context::replaceSingleToken(const std::vector<std::string>& tokens) {
	singleTokens = std::vector <std::string>(tokens);
}

//replace special double token;
void Context::replaceDoubleToken(const std::vector<std::string>& tokens) {
	doubleTokens = std::vector < std::string >(tokens);
}

//----< transition to next state >-----------------------------------
ConsumeState* ConsumeState::nextState()
{
	int chNext = _pContext->_pIn->peek();
	if (_pContext->currChar == EOF) {
		_pContext->_pState = _pContext->_pTerminate;
		return _pContext->_pTerminate;
	}
	if ((_pContext->inDoubleQuotes == true && _pContext->currChar != '"') || (_pContext->inSingleQuotes == true && _pContext->currChar != '\'')) {
		_pContext->_pState = _pContext->_pEatString;
		return _pContext->_pEatString;
	}
	if (std::isspace(_pContext->currChar) && _pContext->currChar != '\n'){
		_pContext->_pState = _pContext->_pEatWhitespace;
		return _pContext->_pEatWhitespace;
	}
	if (_pContext->currChar == '/' && chNext == '/'){
		_pContext->_pState = _pContext->_pEatCppComment;
		return _pContext->_pEatCppComment;
	}
	if (_pContext->currChar == '*' && chNext == '/'){
		_pContext->_pState = _pContext->_pEatCComment;
		return _pContext->_pEatCComment;
	}
	if (_pContext->currChar == '/' && chNext == '*'){
		_pContext->_pState = _pContext->_pEatCComment;
		return _pContext->_pEatCComment;
	}
	if (_pContext->currChar == '\n'){
		_pContext->_pState = _pContext->_pEatNewline;
		return _pContext->_pEatNewline;
	}
	if (std::isalnum(_pContext->currChar) || (_pContext->currChar == '_' && !_pContext->isSpecialSingleChar("_"))){
		_pContext->_pState = _pContext->_pEatAlphanum;
		return _pContext->_pEatAlphanum;
	}
	if (ispunct(_pContext->currChar) && (_pContext->currChar != '_' || (_pContext->currChar == '_' && _pContext->isSpecialSingleChar("_")))){
		_pContext->_pState = _pContext->_pEatPunctuator;
		return _pContext->_pEatPunctuator;
	}
	throw(std::logic_error("invalid type"));
}
/////////////////////////////////////////////////////////////////////
// Derived state that consumes terminated and then stop state in terminated
class EatWhitespace : public ConsumeState{
public:
	EatWhitespace(Context* pContext) { this->_pContext = pContext; };
	virtual void eatChars(){
		_pContext->token.clear();
		do {
			if (!collectChar())  // end of stream
				return;
		} while (std::isspace(_pContext->currChar) && _pContext->currChar != '\n');
	}
};

class StateTerminate : public ConsumeState{
public:
	StateTerminate(Context* pContext) { this->_pContext = pContext; };
	virtual void eatChars(){
		_pContext->token.clear();
		if (_pContext->currChar == EOF) {
			_pContext->token.tokenType = TOKEN_TYPES::TERMINATED;
		}
	}
};

/////////////////////////////////////////////////////////////////////
// Derived state that consumes string
/*
Backup function for escape character
*/
class EatString : public ConsumeState{
public:
	EatString(Context* pContext) { this->_pContext = pContext; };
	virtual void eatChars()
	{
		_pContext->token.clear();
		_pContext->token.tokenType = TOKEN_TYPES::STRING;
		_pContext->token.tokenIndex = _pContext->tokenIndex++;
		_pContext->token.lineNumber = _pContext->lineNumber;

		char next = _pContext->_pIn->peek();
		do {
			std::string s = "  ";
			_pContext->_pIn->seekg(-1, ios_base::cur);
			_pContext->_pIn->read(&s[0], 2);
			_pContext->_pIn->seekg(-1, ios_base::cur);
			std::string whole = utils::escape(s);

			// Deal with special characters for example """ and '''. 
			if (_pContext->currChar == '\\' && next == '\'' && _pContext->inSingleQuotes) { // ' in ' '
				_pContext->token.tokenValue += '\'';
				_pContext->_pIn->get();
			}
			else if (_pContext->currChar == '\\' && next == '"' && _pContext->inDoubleQuotes) { // " in " "
				_pContext->token.tokenValue += '"';
				_pContext->_pIn->get();
			}
			/*else if (_pContext->isSpecialSingleChar(whole)) {
				_pContext->token.tokenValue += whole;
				_pContext->_pIn->get();
			}*/
			else if (_pContext->currChar == '\\') {	// Deal with new line in c++ source code
				_pContext->token.tokenValue += _pContext->currChar;
				collectChar();
				_pContext->token.tokenValue += _pContext->currChar;
			}
			else {
				_pContext->token.tokenValue += _pContext->currChar;
			}
			_pContext->currChar = _pContext->_pIn->get();
			if (!_pContext->_pIn->good())  // end of stream
				return;
			
			next = _pContext->_pIn->peek();
		} while (_pContext->currChar != '"' && _pContext->currChar != '\'');
	}
};

// Derived state that consumes CPP comment
class EatCppComment : public ConsumeState
{
public:
	EatCppComment(Context* pContext) { this->_pContext = pContext; };
	virtual void eatChars()
	{
		_pContext->token.clear();
		if (_pContext->_returnComment) {
			_pContext->token.tokenType = "CppComment";
		}
		else {
			_pContext->token.tokenType = "";
		}
		collectChar();
		collectChar();
		_pContext->token.tokenIndex = _pContext->tokenIndex++;
		_pContext->token.tokenValue += "//";
		//std::cout << "\n  eating C++ comment";
		do {
			_pContext->token.tokenValue += _pContext->currChar;
			if (!collectChar())  // end of stream
				return;
		} while (_pContext->currChar != '\n');
	}
};

// Consume C comment
class EatCComment : public ConsumeState
{
public:
	EatCComment(Context* pContext) { this->_pContext = pContext; };
	virtual void eatChars()
	{
		_pContext->token.clear();
		if (_pContext->_returnComment) {
			_pContext->token.tokenType = "CComment";
		}
		else {
			_pContext->token.tokenType = "";
		}
		collectChar();
		collectChar();
		_pContext->token.tokenValue += "/*";
		_pContext->token.tokenIndex = _pContext->tokenIndex++;
		//std::cout << "\n  eating C comment";
		do {
			_pContext->token.tokenValue += _pContext->currChar;
			if (!collectChar())  // end of stream
				return;
		} while (_pContext->currChar != '*' || _pContext->_pIn->peek() != '/');
		_pContext->token.tokenValue += "*/";
		collectChar();
		collectChar();
	}
};

/////////////////////////////////////////////////////////////////////
// Derived state that consumes punctuators and returns as token
class EatPunctuator : public ConsumeState
{
public:
	EatPunctuator(Context* pContext) { this->_pContext = pContext; };
	virtual void eatChars()
	{
		_pContext->token.clear();
		_pContext->token.tokenType = "Punctuator";
		std::string next = "";
		std::string whole = "";
		_pContext->token.tokenIndex = _pContext->tokenIndex++;
		_pContext->token.lineNumber = _pContext->lineNumber;

		next = _pContext->_pIn->peek();
		whole = std::string(1, _pContext->currChar) + next;
		bool emptyToken = _pContext->token.tokenValue.empty();

		//Is two char special 
		if (std::find(_pContext->doubleTokens.begin(), _pContext->doubleTokens.end(), whole) != _pContext->doubleTokens.end() && emptyToken) {
			_pContext->token.tokenValue = whole;
			collectChar();
			collectChar();
		}
		else if (std::find(_pContext->singleTokens.begin(), _pContext->singleTokens.end(), std::string(1, _pContext->currChar)) != _pContext->singleTokens.end() && emptyToken) {
			_pContext->token.tokenValue = _pContext->currChar;
			collectChar();
		}
		else {
			_pContext->token.tokenValue += _pContext->currChar;
			collectChar();
		}

		//Set variable when begin to deal with quated string
		if (_pContext->token.tokenValue == "\"" && _pContext->inDoubleQuotes == false)
		{
			_pContext->inDoubleQuotes = true;
		}
		else if (_pContext->token.tokenValue == "'" && _pContext->inSingleQuotes == false)
		{
			_pContext->inSingleQuotes = true;
		}
		else if (_pContext->token.tokenValue == "\"" && _pContext->inDoubleQuotes == true)
		{
			_pContext->inDoubleQuotes = false;
		}
		else if (_pContext->token.tokenValue == "'" && _pContext->inSingleQuotes == true)
		{
			_pContext->inSingleQuotes = false;
		}
	}
};

/////////////////////////////////////////////////////////////////////
// Derived state that consumes alphanumerics and returns as token
class EatAlphanum : public ConsumeState
{
public:
	EatAlphanum(Context* pContext) { this->_pContext = pContext; };
	virtual void eatChars()
	{
		_pContext->token.clear();
		_pContext->token.tokenType = "Alphanum";
		_pContext->token.tokenIndex = _pContext->tokenIndex++;
		_pContext->token.lineNumber = _pContext->lineNumber;

		do {
			_pContext->token.tokenValue += _pContext->currChar;
			if (!this->collectChar())  // end of stream
				return;
		} while (isalnum(_pContext->currChar) || _pContext->currChar == '_');
		_pContext->_pState = nextState();
	}
};

/////////////////////////////////////////////////////////////////////
// Derived state that consumes newline and returns as single
// character token.
class EatNewline : public ConsumeState
{
public:
	EatNewline(Context* pContext) { this->_pContext = pContext; };
	virtual void eatChars()
	{
		_pContext->token.clear();
		_pContext->token.tokenType = TOKEN_TYPES::NEWLINE;
		_pContext->token.tokenValue = "\n";
		//_pContext->token.tokenIndex = _pContext->tokenIndex++;
		_pContext->token.lineNumber = _pContext->lineNumber;
		if (!collectChar())  // end of stream
			return;
	}
};

//----< base class member function to create derived states >--------
ConsumeState::ConsumeState(){ }

//----< base class member function to create derived states >--------
ConsumeState::ConsumeState(Context* pContext) { this->_pContext = pContext; }

//Deconstruct
ConsumeState::~ConsumeState(){ }

Context::Context() {
	_pEatCppComment = new EatCppComment(this);
	_pEatCComment = new EatCComment(this);
	_pEatWhitespace = new EatWhitespace(this);
	_pEatPunctuator = new EatPunctuator(this);
	_pEatAlphanum = new EatAlphanum(this);
	_pEatNewline = new EatNewline(this);
	_pEatString = new EatString(this);
	_pTerminate = new StateTerminate(this);
	_pState = _pEatWhitespace;

	inDoubleQuotes = false;
	inSingleQuotes = false;
	lineNumber = 1;
	tokenIndex = 1;
	singleTokens = { "\"", "'", "<" , ">", "[", "]","{","}","(",")", ":", "=", "+", "-", "*", "/", "!" };
	doubleTokens = { "<<", ">>", "::", "==", "+=", "-=", "*=", "/=", "\n" };
}

Context::~Context() {
	delete _pEatCppComment;
	delete _pEatCComment;
	delete _pEatWhitespace;
	delete _pEatPunctuator;
	delete _pEatAlphanum;
	delete _pEatNewline;
	delete _pEatString;
	delete _pTerminate;
}

// Determine if a string is a special single characters
bool Context::isSpecialSingleChar(const char&c) {
	auto s = to_string(c);
	return isSpecialCharPair(s);
}

// Determine if a string is a special single characters
bool Context::isSpecialSingleChar(const string &c) {
	return singleTokens.end() != std::find(singleTokens.begin(), singleTokens.end(), c);
}

// Determine if a string is a special double characters
bool Context::isSpecialCharPair(const string &s) {
	return doubleTokens.end() != std::find(doubleTokens.begin(), doubleTokens.end(), s);

}
//----< initialize the toker to start with EatWhitespace >-----------
Toker::Toker(){
	_pContext = new Context();
	pConsumer = _pContext->_pEatWhitespace;
	applyConfig();
}

// Deconstruct
Toker::~Toker() { 
	delete _pContext;
}

void Toker::returnComments(bool returnComments ) {
	_pContext->_returnComment = returnComments;
}

// Read configuration from file and intialize the configure items
void Toker::applyConfig() {
	std::string fileSpec = "token_config.txt";
	std::ifstream in(fileSpec);
	char *line = new char[1000];
	
	for (std::string line; std::getline(in, line); ) {
		if (line.find("single_character_token") != -1) {
			int beginIndex = line.find_first_of('"');
			std::string configValue = line.substr(beginIndex + 1, line.size() - beginIndex - 2  );
			auto singleTokens = split(configValue, ',');
			std::transform(singleTokens.begin(), singleTokens.end(), singleTokens.begin(), trimL);
			setSpecialSingleChars(singleTokens, true);
		}
		else if (line.find("double_character_token") != -1) {
			int beginIndex = line.find_first_of('"');
			std::string configValue = trimL(line.substr(beginIndex + 1, line.size() - beginIndex - 2 ));
			auto doubleTokens = split(configValue, ',');
			std::transform(doubleTokens.begin(),doubleTokens.end(), doubleTokens.begin(), trimL);
			setSpecialCharPairs(doubleTokens, true);
		}
	}
}
	
// Attach a stream to Tokenizer and use getTok to parse and return token
bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		_pContext->_pIn = pIn;
		pConsumer = _pContext->_pEatWhitespace;
		return true;
	}
	return false;
}
int Toker::getCurrentLineNumber() {
	return _pContext->lineNumber;
}
// function to get token, this function will return a token every time it is invoked.
//----< this method will be called by SemiExpression >---------------
Token Toker::getTok()
{
	while (true)
	{
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;

		////insurance
		//if (!pConsumer->canRead())
		//{
		//	auto tok = Token();
		//	tok.tokenType = TOKEN_TYPES::TERMINATED;
		//	return tok;
		//}
	}
	return pConsumer->getTok();
}

// Tell whether the consumer can continue to read stream and return token
bool Toker::canRead() { return pConsumer->canRead(); }

// Set special single character
// Parameters:
//    chars: vector of special single character such like ':', '!'
//    replace: whether to replace the old special single characters with the new configuration
void Toker::setSpecialSingleChars(const std::vector<std::string> chars, bool replace) {
	size_t i = 0;
	auto copyChars = std::vector<std::string>(chars);
	for (i = 0; i < copyChars.size(); i++) {
		copyChars[i] = utils::escape(copyChars[i]);
		if (copyChars[i].size() != 1) {
			copyChars.erase(copyChars.begin() + i);
			continue;
		}
	}
	
	if (replace) {
		_pContext->replaceSingleToken(copyChars);
	}
	else {
		for (auto it = copyChars.begin(); it != copyChars.end(); it++) {
			std::string s = *it;
			auto nowTokens = _pContext->getSingleTokens();
			if (nowTokens.end() == std::find(nowTokens.begin(), nowTokens.end(), s) ) {
				_pContext->addSingleToken(s);
			}
		}
	}
}

// Set special character pairs
// Parameters:
//    chars: vector of special characters such like '::', '{{'
//    replace: whether to replace the old special character pairs with the new configuration
void Toker::setSpecialCharPairs(const std::vector<std::string> chars, bool replace) {
	size_t i = 0;
	auto copyChars = std::vector<std::string>(chars);
	for (i = 0; i < copyChars.size(); i++) {
		if (copyChars[i].size() != 2) {
			copyChars.erase(copyChars.begin() + i);
		}
	}
	if (replace) {
		_pContext->replaceDoubleToken(copyChars);
	}
	else {
		for (auto it = copyChars.begin(); it != copyChars.end(); it++) {
			std::string s = *it;
			auto nowTokens = _pContext->getSingleTokens();
			if (nowTokens.end() == std::find(nowTokens.begin(), nowTokens.end(), s)) {
				_pContext->addDoubleToken(s);
			}
		}
	}
}

// Clear current pConsumer state to enable another file to attach and parse
void Toker::clear() {
	_pContext->clear();
}

// Get special single characters
std::vector<std::string> Toker::getSpecialSingleChars() {
	return std::vector<std::string>(_pContext->getSingleTokens());
}

// Get special double characters
std::vector<std::string> Toker::getSpecialCharPairs() {
	return std::vector<std::string>(_pContext->getDoubleTokens());
}


//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	std::string fileSpec = "../Tokenizer/mixed.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	while (in.good())
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	}
	std::cout << "\n\n";

	char str[256];
	std::cin.get(str, 256);
	return 0;
}

#endif
