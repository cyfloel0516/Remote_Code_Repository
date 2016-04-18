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
	// Process header
	auto line = socket_.recvString('\n');
	this->handleHeader(line);
	while (true) {
		line = socket_.recvString('\n');
		auto command = HttpUtils::getCommand(line);
		if (command == "Content-Type") {
			this->handleContentType(line);
		}
		else if (command == "Content-Length") {
			this->handleContentLength(line);
		}
		else if (line == this->request.boundary) {
			
		}
	}
}

void HttpRequestHandler::handleContentLength(std::string line) {
	this->request.ContentLength = std::stoi(HttpUtils::getValue(line));
}

void HttpRequestHandler::handleHeader(std::string line)
{
}

void HttpRequestHandler::handleContentFiles(Socket& socket_){
	while (true) {
		auto lineData = HttpUtils::getLineData(socket_.recvString('\n'));
		if (lineData.Name == "Content-Disposition") {
			// Process filename
			//auto filename = lineData
		}
	}
}

void HttpRequestHandler::handleContentType(std::string line) {
	//this->request.ContentType = HttpUtils::getValue(line);
	if (line.find(";") != std::string::npos) {
		auto keyValues = Utilities::StringHelper::split(line, ';');
		for (auto keyValue : keyValues) {
			std::vector<std::string> pair;
			if(keyValue.find(':') != std::string::npos)
				pair = Utilities::StringHelper::split(keyValue, ':');
			else if(keyValue.find('=') != std::string::npos)
				pair = Utilities::StringHelper::split(keyValue, '=');
			auto key = pair[0];
			auto value = pair[1];
			if (key == "Content-Type") this->request.ContentType = value;
			else if (key == "boundary") this->request.boundary = value;
		}
	}
	else {
		this->request.ContentType = HttpUtils::getValue(line);
	}
}


//----< test stub starts here >----------------------------------------------

int main()
{

	try
	{
		SocketSystem ss;
		SocketListener sl(9080, Socket::IP4);
		HttpRequestHandler cp;
		sl.start(cp);

		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{

	}
}

