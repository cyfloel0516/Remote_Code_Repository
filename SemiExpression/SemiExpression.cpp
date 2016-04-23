/*
* Tokenizer.cpp: Implementation of semi expression
* version: 1.0
* Language: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
* Source: Jim Fawcett, Syracuse University, CST 4-187
*/

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExpression.h"
#include "../Tokenizer/Tokenizer.h"
#include "SemiExpression.h"
#include <algorithm>
#include "../Tokenizer/Utils.h"

using namespace Scanner;
using namespace utils;

// Constructor
SemiExpression::SemiExpression(){
	applyConfig();
}

SemiExpression::SemiExpression(Toker* pToker) : _pToker(pToker) {
	applyConfig();
}

// Determite if to can stop collecting tokens
bool SemiExpression::ifToStopCollect(const Token& token) {
	bool stop = false;
	auto tokenType = token.tokenType;
	auto tokenValue = token.tokenValue;
	if (tokenType == TOKEN_TYPES::PUNCTUATOR && tokenValue == "{"
		|| tokenType == TOKEN_TYPES::PUNCTUATOR && tokenValue == ";"
		|| tokenType == TOKEN_TYPES::PUNCTUATOR && tokenValue == "}"
		)
		stop = true;
	else if (tokenType == TOKEN_TYPES::NEWLINE && _tokens.size() > 1 ) {
		bool first = true;
		for (size_t i = 0; i < _tokens.size(); i++) {
			if (_tokens[i].tokenType == TOKEN_TYPES::NEWLINE) 
				continue;
			else if (_tokens[i].tokenType != TOKEN_TYPES::NEWLINE && _tokens[i].tokenValue == "#" && _tokens[i].tokenType == TOKEN_TYPES::PUNCTUATOR) {
				stop = true;
				break;
			}
			else {
				break;
			}
		}
	}
	else if (tokenType == TOKEN_TYPES::PUNCTUATOR && tokenValue == ":"
		&& _tokens[_tokens.size() - 2].tokenType == TOKEN_TYPES::ALPHANUM
		&& (_tokens[_tokens.size() - 2].tokenValue == "private" || _tokens[_tokens.size() - 2].tokenValue == "protected" || _tokens[_tokens.size() - 2].tokenValue == "public")) {
		stop = true;
	}
	return stop;
}

// Tell if a token is a comment
bool SemiExpression::isComment(const Scanner::Token& tok)
{
	return tok.tokenType == TOKEN_TYPES::CPPCOMMENT || tok.tokenType == TOKEN_TYPES::CCOMMENT;
}

bool SemiExpression::isForLoop()
{
	size_t forIndex = find("for");
	size_t openParenIndex = find("(");
	if (forIndex < _tokens.size() 
		&& _tokens[forIndex].tokenType == TOKEN_TYPES::ALPHANUM
		&& openParenIndex < _tokens.size()
		&& _tokens[_tokens.size() - 1].tokenType == TOKEN_TYPES::PUNCTUATOR 
		&& _tokens[_tokens.size() - 1].tokenValue == ";") {
		//second colon
		collect(false);
		if (!(_tokens[_tokens.size() - 1].tokenType == TOKEN_TYPES::PUNCTUATOR && _tokens[_tokens.size() - 1].tokenValue == ";")) { 
			return false; 
		}
		//next terminator
		collect(false);
		size_t closeParenPosition = find(")");
		if (closeParenPosition >= _tokens.size()) {
			return true;
		}
		else {
			return false;
		}
		return true;
	}
	return false;
}

// This is to get token from Tokenizer and return the tokens.
// parameters:
//   clear: whethere to clear current collection
//          if ture, it will clear all tokens in current token collection
// return:
//    true: there are more tokens in codes
//    false: it is reach the end of codes
bool SemiExpression::get(bool clear)
{
	bool next = collect(clear);

	isForLoop();

	if (!_returnNewline) {
		removeNewline();
	}

	if (!_returnComment) {
		removeComment();
	}
	return next;
}

bool SemiExpression::collect(bool clear) {
	// Clear token collection
	if (clear) {
		_tokens.clear();
	}
	bool forLoop = false;
	while (true)
	{
		Token token = _pToker->getTok();
		if (token.tokenType == TOKEN_TYPES::TERMINATED)
			break;
		_tokens.push_back(token);

		if (ifToStopCollect(token))
			return true;
	}
	return false;
}

// Return token in index n
Token SemiExpression::operator[](size_t n)
{
	if (n < 0 || n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

// Find a token based on token value
// Return:
//    size_t: index of the token
size_t Scanner::SemiExpression::find(const std::string & tok)
{
	int count = 0;
	for (auto token : _tokens) {
		if (token.tokenValue == tok) {
			return count;
		}
		count++;
	}
	return length();
}

// Return the size of current token collection
size_t SemiExpression::length()
{
	return _tokens.size();
}

std::string SemiExpression::toString() {
	std::string result;
	for (size_t i = 0; i < length(); i++) {
		if (_tokens[i].tokenType != TOKEN_TYPES::NEWLINE) {
			result += _tokens[i].tokenValue + " ";
		}
	}
	return result;
}

void SemiExpression::returnNewline(bool returnNewline) {
	_returnNewline = returnNewline;
}

void SemiExpression::removeNewline() {
	for (size_t i = 0; i < _tokens.size();i++) {
		if (_tokens[i].tokenType == TOKEN_TYPES::NEWLINE) {
			_tokens.erase(_tokens.begin() + i, _tokens.begin() + i + 1);
			i--;
		}
	}
}

void SemiExpression::returnComment(bool returnComment) {
	_returnComment = returnComment;
}

void SemiExpression::removeComment() {
	for (size_t i = 0; i < _tokens.size();i++) {
		if (_tokens[i].tokenType == TOKEN_TYPES::CCOMMENT || _tokens[i].tokenType == TOKEN_TYPES::CPPCOMMENT) {
			_tokens.erase(_tokens.begin() + i, _tokens.begin() + i + 1);
			i--;
		}
	}
}

// Print out semi-expression result to command line
// Parameters:
//    bool showNewLines: if show new line
std::string SemiExpression::show(bool showNewLines)
{
	std::ostringstream out;
	out << "\n  ";
	for (auto token : _tokens)
		if ((token.tokenValue != "\n" && token.tokenType != TOKEN_TYPES::NEWLINE) || showNewLines)
			out << token.tokenValue << " ";
	out << "\n";
	return out.str();
}

// Push a token to the end of current token collection
void Scanner::SemiExpression::push_back(const Token tok)
{
	_tokens.push_back(tok);
}

// Un-implemented
bool Scanner::SemiExpression::merge(const Token firstTok, const Token secondTok)
{
	return false;
}

// Remove a token by token value
bool Scanner::SemiExpression::remove(const std::string& value){
	int index = 0;
	for (auto token : _tokens) {
		if (token.tokenValue == value) {
			_tokens.erase(_tokens.begin() + index);
			return true;
		}
		index++;
	}
	return false;
}

// Remove a token by token value and token type
bool Scanner::SemiExpression::remove(const Token tok)
{
	int count = 0;
	for (auto token : _tokens) {
		if (token.tokenType == tok.tokenType && token.tokenValue == tok.tokenValue) {
			_tokens.erase(_tokens.begin() + count);
			return true;
		}
		count++;
	}
	return false;
}

// Remove element in specified position
// Parameters:
//    size_t t: index of the element
bool Scanner::SemiExpression::remove(size_t i)
{
	if (i < _tokens.size()) {
		_tokens.erase(_tokens.begin() + i);
		return true;
	}
	return false;
}

// Change all token value to lower case
void Scanner::SemiExpression::toLower(){
	size_t i = 0;
	for (i = 0; i < _tokens.size(); i++) {
		auto token = _tokens.begin() + i;
		std::transform(token->tokenValue.begin(), token->tokenValue.end(), token->tokenValue.begin(), ::tolower);

	}
}

// Remove all newline and space in front of a string
void Scanner::SemiExpression::trimFront(){
	for (auto token : _tokens) {
		token.tokenValue = trimL(token.tokenValue);
	}
}

// Clear token collection
void Scanner::SemiExpression::clear()
{
	_tokens = std::vector<Token>();
}

// Compare if two ignore sequence are equal.
bool Scanner::SemiExpression::compareSequence(std::vector<string> s1, std::vector<string> s2) {
	if (s1.size() != s2.size()) {
		return false;
	}
	else {
		size_t i;
		for (i=0; i < s1.size(); i++) {
			if (s1[i] != s2[2]) {
				return false;
			}
		}
	}
	return true;
}

// Set ignore sequence by passing ignore sequence
// Parameters:
//    sequence: vector of string of ignore sequence
//    replace: if to replace old configuration
void Scanner::SemiExpression::setTerminator(std::vector<std::string> sequences, bool replace) {
	if (replace) {
		ignoreSequences = std::vector<std::vector<std::string>>();
	}
	for (auto s : sequences ) {
		if (!trimL(s).empty()) {
			std::string configValue = trimL(s);
			auto ignores = convertStringToVector(configValue);
			bool duplicated = false;
			for (auto s : ignoreSequences) {
				if (compareSequence(s, ignores)) {
					duplicated = true;
					break;
				}
			}
			if(!duplicated)
				ignoreSequences.push_back(ignores);
		}
	}
}

// Apply configuration from config file
void Scanner::SemiExpression::applyConfig() {
	std::string fileSpec = "./ignore_config.txt";
	std::ifstream in(fileSpec);

	vector<std::string> sequences;
	for (std::string line; std::getline(in, line); ) {
		if (!trimL(line).empty()) {
			std::string configValue = trimL(line);
			sequences.push_back(configValue);
		}
	}
	setTerminator(sequences);
}

// Convert a string to vector, split by some specific string.
std::vector<std::string> Scanner::SemiExpression::convertStringToVector(std::string s) {
	s = trimL(s);
	auto vectorS = split(s, ' ');
	return vectorS;
}

std::vector<vector<std::string>> Scanner::SemiExpression::getIgnoreSequence() {
	return std::vector<std::vector<std::string>>(ignoreSequences);
}


//----< test stub >--------------------------------------------------

#ifdef TEST_SEMIEXPRESSION

#include <fstream>
int main()
{
	Toker toker;
	std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker.attach(&in);

	SemiExpression semi(&toker);
	while (semi.get())
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (semi.length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
		std::cout << "\n\n";
	}
	return 0;
}
#endif