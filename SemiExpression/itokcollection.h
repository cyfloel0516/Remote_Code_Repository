/*
* Tokenizer.cpp: Semi Expression Interface
* version: 1.0
* Language: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
* Source: Jim Fawcett, Syracuse University, CST 4-187
*/
#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H


#include "../Tokenizer/Tokenizer.h"
#include <string>
using namespace std;

namespace Scanner {
	struct ITokCollection
	{
		virtual bool get(bool clear = true) = 0;
		virtual size_t length() = 0;
		virtual Token operator[](size_t n) = 0;
		virtual size_t find(const std::string& tok) = 0;
		virtual void push_back(const Token tok) = 0;
		virtual bool merge(const Token firstTok, const Token secondTok) = 0;

		virtual bool remove(const std::string& value) = 0;
		virtual bool remove(const Token tok) = 0;
		virtual bool remove(size_t i) = 0;

		virtual void toLower() = 0;
		virtual void trimFront() = 0;
		virtual void clear() = 0;
		virtual std::string show(bool showNewLines = false) = 0;
		virtual bool isComment(const Scanner::Token& tok) = 0;
		virtual ~ITokCollection() {};

		virtual void returnNewline(bool returnNewline = false) = 0;
		virtual void returnComment(bool returnComment = false) = 0;
		virtual std::string toString() = 0;
	};

}
#endif