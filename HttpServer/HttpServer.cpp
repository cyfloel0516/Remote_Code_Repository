/*
*	HttpServer.cpp
*	Implementation of HttpServer
*
*	Author: Junkai Yang
*/
#include "HttpServer.h"
#include "../Utilities/Utilities.h"
#include "../HttpBase/HttpUtils.h"
#include "../HttpBase/HttpPacket.h"
#include "../FileSystem_Windows/FileSystem.h"

void HttpRequestHandler::operator()(Socket& socket_) {
	// Process header
	auto line = socket_.recvString('\n');
	this->handleHeader(line);
	while (socket_.bytesWaiting() > 0) {
		line = socket_.recvString('\n');
		auto command = HttpUtils::getCommand(line);
		if (command == "Content-Type") {
			this->handleContentType(line);
		}
		else if (command == "Content-Length") {
			this->handleContentLength(line);
		}
		else if (line == this->request.boundary) {
			this->handleContentFiles(socket_);
		}
	}
	// Look up for the route table and let the handler to handle specified request
	auto route = this->routeTable.find(this->request.Resource);
	if (route != this->routeTable.end()) {
		// Route existed
		auto response = route->second(this->request);
	}
}

void HttpRequestHandler::handleContentLength(std::string line) {
	this->request.ContentLength = std::stoi(HttpUtils::getValue(line));
}

void HttpRequestHandler::handleHeader(std::string line){
	std::vector<std::string> headers = Utilities::StringHelper::split(line, ' ');
	if (headers.size() > 2) {
		this->request.Type = headers[0];
		this->request.Resource = headers[1];
	}
}

void HttpRequestHandler::handleContentFiles(Socket& socket_){
	std::string filename;
	bool inFile = false;
	HttpRequestLine lineData;
	while (socket_.bytesWaiting() > 0) {
		auto tempLineData = HttpUtils::getLineData(socket_.recvString('\n'));
		if (tempLineData.Name == "Content-Disposition") {
			// Process filename
			filename = tempLineData.Proterties["filename"];
			lineData = tempLineData;
			inFile = true;
		}
		else if (inFile && tempLineData.Name.empty()) {
			//std::string filePath = "../repository/" + filename;
			//FileSystem::File file(filePath);
			//file.open(FileSystem::File::out, FileSystem::File::binary);
			std::string fileContent = socket_.recvString(this->request.boundary);
			std::ostringstream s;
			s.str(fileContent);
			request.files.insert({ filename, &s });
			//file.putBuffer(fileContent.size(), &fileContent[0]);
			//file.close();
			inFile = false;
		}
	}
}

void HttpRequestHandler::setHttpRequest(Socket & socket_)
{
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
			this->handleContentFiles(socket_);
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
			auto key = Utilities::StringHelper::trim(pair[0]);
			auto value = Utilities::StringHelper::trim(pair[1]);
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

