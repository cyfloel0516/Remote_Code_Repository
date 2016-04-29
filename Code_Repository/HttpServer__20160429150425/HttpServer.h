#pragma once
/*
	HttpServer.h
	This package is used to build a simple server to handle HTTP liked message.
	It uses Sockets for underlying support.
	There are three steps for HttpServer
	1.	Receive a message and deserialize it to a HttpRequest instance
	2.	Find route table which match the resource identifier in the request instance (URL), redirect the request to the registered handler
	3.	Get the response from handler, serialize it and return it to client

	Author: Junkai Yang
*/
#include <string>
#include <iostream>
#include <functional>

#include "../Sockets/Sockets.h"
#include "../HttpBase/HttpPacket.h"

using namespace std;

// Http Request Object, contains some necessary value for a request
class HttpRequestHandler
{
public:
	// Called by Socket, handle the socket connection
	void operator()(Socket& socket_);

	// Add route rule to route table
	void addRoute(std::string url, std::function<HttpResponse(HttpRequest)> handler);
	
private:
	// Handle Content Type, set the content-type value in request object
	void handleContentType(std::string line);

	// Handle Content Length, set the content-length value in request object
	void handleContentLength(std::string line);

	// Handle the header of request
	void handleHeader(std::string line);

	// Handle the files
	void handleContentFiles(Socket& socket_);

	// Request object for this socket connection
	HttpRequest request;

	// Read content from socket and make a coresponding HttpRequest object
	void setHttpRequest(Socket& socket);

	// Store the route information
	std::map<std::string, std::function<HttpResponse(HttpRequest)>> routeTable;
};