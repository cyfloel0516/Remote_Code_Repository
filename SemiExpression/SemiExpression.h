/*
* Tokenizer.cpp: header file of semi expression
* version: 1.0
* Language: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
* Source: Jim Fawcett, Syracuse University, CST 4-187
*/
#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H


#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "itokcollection.h"
#include <queue>

namespace Scanner
{
	class SemiExpression: public ITokCollection
	{
	public:
		SemiExpression();
		SemiExpression(Toker* pToker);

		size_t length();

		// Inherited via ITokCollection

		// Get a collection of tokens
		virtual bool get(bool clear = true) override;
		// Get token in collection by index
		virtual Token operator[](size_t n) override;
		// find a token by value
		virtual size_t find(const std::string & tok) override;
		// Push a token to current collection
		virtual void push_back(const Token tok) override;
		// Unimplemented
		virtual bool merge(const Token firstTok, const Token secondTok) override;
		// Remove a token by value
		virtual bool remove(const std::string& value) override;
		// Remove a token by token structure
		virtual bool remove(const Token tok) override;
		// Remove a token by index
		virtual bool remove(size_t i) override;
		// Transform all token value to lower case
		virtual void toLower() override;
		// Trim front of token value
		virtual void trimFront() override;
		// Clear token collection
		virtual void clear() override;
		// Print out token collection
		virtual std::string show(bool showNewLines = false) override;
		// Set terminator of token collection
		void setTerminator(std::vector<string> sequences, bool replace = false);
		bool isComment(const Scanner::Token& tok) override;
		// Get ignoreSequence
		std::vector<vector<std::string>> getIgnoreSequence();
		bool isForLoop();
		bool collect(bool clear);
		std::string toString();
		void returnNewline(bool returnNewline = false) ;
		void removeNewline();

		void returnComment(bool returnComment = false);
		void removeComment();

	private:
		bool _returnNewline = false;
		bool _returnComment = false;
		std::vector<vector<std::string>> ignoreSequences = std::vector<std::vector<std::string>>();
		std::vector<Token> _tokens;
		Toker* _pToker;
		void applyConfig();
		std::vector<std::string> convertStringToVector(std::string s);
		bool compareSequence(std::vector<string> s1, std::vector<string> s2);
		int bufferIndex = 0;
		bool ifToStopCollect(const Token& token);
	};
}
#endif