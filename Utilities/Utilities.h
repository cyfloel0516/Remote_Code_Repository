#ifndef UTILITIES_H
#define UTILITIES_H
///////////////////////////////////////////////////////////////////////
// Utilities.h - small, generally useful, helper classes             //
// ver 1.2                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes StringHelper and Converter and a global
* function putline().  This class will be extended continuously for 
* awhile to provide convenience functions for general C++ applications.
*
* Build Process:
* --------------
* Required Files: Utilities.h, Utilities.cpp
*
* Build Command: devenv Utilities.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.2 : 22 Feb 2015
* - changed default underline char in Title(const std::string&, Char)
* ver 1.1 : 06 Feb 2015
* - fixed bug in split which turns a comma separated string into
*   a vector of tokens.
* - added comments
* ver 1.0 : 05 Feb 2016
* - first release
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/
#include <string>
#include <vector>
#include <sstream>
#include <functional>

namespace Utilities
{
  class StringHelper
  {
  public:
    static std::vector<std::string> split(const std::string& src);
    static void Title(const std::string& src, char underline = '=');
    static void title(const std::string& src);

	// Split a string based on a specified character
	static std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems);

	// Split a string based on a specified character
	static std::vector<std::string> split(const std::string &s, char delim);

	// Trim left part of a string
	static std::string trimL(const std::string &s);

	// Trim right part of a string
	static std::string trimR(const std::string &s);

	// Trim left and right part of a string
	static std::string trim(const std::string &s);

	// Replace some content in a string with other content
	static std::string replace(const std::string& str, const std::string& from, const std::string& to);

	// Change a string into escaped string
	static std::string escape(const std::string& str);

	// Change a escaped string back to un escaped style
	static std::string un_escape(const std::string& str);

  };

  void putline();

  template <typename T>
  class Converter
  {
  public:
    static std::string toString(const T& t);
    static T toValue(const std::string& src);
  };

  template <typename T>
  std::string Converter<T>::toString(const T& t)
  {
    std::ostringstream out;
    out << t;
    return out.str();
  }

  template<typename T>
  T Converter<T>::toValue(const std::string& src)
  {
    std::istringstream in(src);
    T t;
    in >> t;
    return t;
  }
}
#endif
