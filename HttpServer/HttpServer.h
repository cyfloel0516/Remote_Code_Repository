#pragma once
/*
	HttpServer.h
	This package is used to build a simple server to handle HTTP liked message.
	It uses Sockets for underlying support.

	Author: Junkai Yang
*/
#include <string>
#include <iostream>
#include "../Sockets/Sockets.h"

class HttpRequestHandler
{
public:
	void operator()(Socket& socket_);

private:
	void handleType(std::string line);

	std::string getCommand(std::string line);
};