/*
*	HttpServer.cpp
*	Implementation of HttpServer
*
*	Author: Junkai Yang
*/
#include "HttpServer.h"
#include "../Utilities/Utilities.h"
#include "../HttpBase/HttpUtils.h"
void HttpRequestHandler::operator()(Socket& socket_) {
	while (true) {
		std::string line = socket_.recvString('\n');
		auto command = HttpUtils::getCommand(line);
		if (command == "Content-Type") {
			this->handleContentType(line);
		}
		else if (command == "Content-Length") {
			this->handleContentLength(line);
		}
	}
}

void HttpRequestHandler::handleContentLength(std::string line) {
	this->request.ContentLength = std::stoi(HttpUtils::getValue(line));
}

void HttpRequestHandler::handleContentType(std::string line) {
	this->request.ContentType = HttpUtils::getValue(line);
}


//----< test stub starts here >----------------------------------------------

int main()
{

	try
	{
		SocketSystem ss;
		SocketListener sl(9080, Socket::IP6);
		HttpRequestHandler cp;
		sl.start(cp);

		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{

	}
}

