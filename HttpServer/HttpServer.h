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
#include "../HttpBase/HttpPacket.h"
// Http Request Object, contains some necessary value for a request

class HttpRequestHandler
{
public:
	// Called by Socket, handle the socket connection
	void operator()(Socket& socket_);

private:
	// Handle Content Type, set the content-type value in request object
	void handleContentType(std::string line);

	// Handle Content Length, set the content-length value in request object
	void handleContentLength(std::string line);

	// Request object for this socket connection
	HttpRequest request;
};