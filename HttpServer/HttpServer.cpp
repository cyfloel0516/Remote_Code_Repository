/*
*	HttpServer.cpp
*	Implementation of HttpServer
*
*	Author: Junkai Yang
*/
#include "HttpServer.h"

void HttpRequestHandler::operator()(Socket& socket_) {
	while (true) {
		std::string line = socket_.recvString('\n');
		
	}
}

std::string HttpRequestHandler::getCommand(std::string line) {
	int pos = line.find(':');
	std::string command;
	if (pos != std::string::npos) {
		command = line.substr(0, pos);
		command = 
	}
}