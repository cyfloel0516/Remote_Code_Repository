#pragma once
/*
*	HttpUtils.h
*	This package includes some useful tools to handle http request and response.
*
*	Author: Junkai Yang
*/

#include <string>
#include <vector>
#include <map>
#include "HttpPacket.h"

class HttpUtils {
public:
	// Get the command type from a request line
	static std::string getCommand(std::string line);

	// Get the command value from a request line
	static std::string getValue(std::string line);

	// Serialize the request object to char array for transfer via HTTP connect
	static std::string serialize(HttpRequest request);

	// Get file contents for transfer
	static std::vector<char> getFileContent(std::string fileInfo);

	// Get command values after command definition
	static HttpRequestLine getLineData(std::string line);

	// Put string into a char vector
	static void putStringToVector(std::vector<char>& destination, std::string value);
};