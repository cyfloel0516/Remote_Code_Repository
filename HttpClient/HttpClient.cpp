/*
*	HttpClient.cpp
*	Implementation of HttpClient.h
*
*	Author: Junkai Yang
*/
#include "HttpClient.h"
#include "../HttpBase/HttpUtils.h"
HttpClient::~HttpClient()
{
	//this->connector.shutDown();
}

void HttpClient::setConnection(std::string address, int port)
{
	this->address = address;
	this->port = port;
}

HttpResponse HttpClient::sendRequest(std::vector<std::string> filePaths)
{
	this->connector.connect(address, port);
	HttpRequest request;
	request.Type = "PUT";
	request.ContentType = "files";
	request.files = filePaths;
	request.boundry = "--------------------------JukaiYangCIS687";
	auto s = HttpUtils::serialize(request);
	this->connector.sendString(s);
	this->connector.recvString('\n');
	return HttpResponse();
}
