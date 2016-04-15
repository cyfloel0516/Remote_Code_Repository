/*
*	HttpServer.cpp
*	Implementation of HttpServer
*
*	Author: Junkai Yang
*/
#include "HttpServer.h"
#include "../Utilities/Utilities.h"
void HttpRequestHandler::operator()(Socket& socket_) {
	while (true) {
		std::string line = socket_.recvString('\n');
		
	}
}

std::string HttpRequestHandler::getCommand(std::string line) {
	int pos = line.find(':');
	std::string command;
	if (pos != std::string::npos) {
		command = Utilities::StringHelper::trim(line.substr(0, pos));
		//if(commad == )
	}
}