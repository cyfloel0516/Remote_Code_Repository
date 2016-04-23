/*
* Tokenizer.h: read words from a file and generate different types of token
* version: 1.0
* Languange: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
* Source: Jim Fawcett, Syracuse University, CST 4-187
*/
/*
* Package Operations:
* -------------------
* This package provides a public Toker class and private ConsumeState class.
* Toker reads words from a std::stream, throws away whitespace and comments
* and returns words from the stream in the order encountered.  Quoted
* strings and certain punctuators and newlines are returned as single tokens.
*
* This is a new version, based on the State Design Pattern.  Older versions
* exist, based on an informal state machine design.
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp
* Build Command: devenv Tokenizer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.1 : 01 Jan 2016
* - added comments and proprocessor guard around test stub
* ver 3.0 : 11 Jun 2014
* - first release of new design
*
* Planned Additions and Changes:
* ------------------------------
* - Return quoted strings as single token
* - Return "[", "]", "{", "}", "(", ")"
*   as single character tokens
*/

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iosfwd>
#include <string>
#include <vector>

using namespace std;

namespace Scanner {
	
	//Token struct
	struct Token {
		// The type of the token
		std::string tokenType;
		//The value of the token
		std::string tokenValue;
		// Indicate this token is from which line
		int lineNumber;
		// Indicate this token's index in its line
		int tokenIndex;
		// Clear token type and value
		void clear();
		Token();
	};

	// Struct defines all types of token
	class TOKEN_TYPES {
	public:
		static const std::string TERMINATED;
		static const std::string STRING;
		static const std::string CPPCOMMENT;
		static const std::string CCOMMENT;
		static const std::string NEWLINE;
		static const std::string SPACE;
		static const std::string PUNCTUATOR;
		static const std::string ALPHANUM;
	};

	class ConsumeState;
	class Context;
	// Tokenizer class to parse a stream and return a token in every get function invokation
	class Toker
	{
	public:
		Toker();
		~Toker();
		// Attach a stream to tokenizer and then can use getTok to get token
		bool attach(std::istream* pIn);
		// Method to get token
		Token getTok();
		// Test if current stream is good and can be read
		bool canRead();
		// Use this method to set single special character
		void setSpecialSingleChars(const std::vector<std::string> chars, bool replace = false);
		// Use this method to set double special character
		void setSpecialCharPairs(const std::vector<std::string> chars, bool replace = false);
		// Currently we use static members to store the state of tokenizer, this method is to clear
		// the static states for that you can use this tokenizer again
		void clear();
		// Return current special single character
		std::vector<std::string> getSpecialSingleChars();
		// Return current special double character
		std::vector<std::string> getSpecialCharPairs();

		void returnComments(bool returnComments = false);

		int getCurrentLineNumber();
	private:
		ConsumeState* pConsumer;
		Token tokens;
		//configurations
		bool storeComments = true;
		void applyConfig();
		Context* _pContext;
	};

}
#endif

