/*
*	HttpClient.cpp
*	Implementation of HttpClient.h
*
*	Author: Junkai Yang
*/
#include "HttpClient.h"

HttpClient::~HttpClient()
{
	//this->connector.shutDown();
}

void HttpClient::setConnection(std::string address, int port)
{
	this->address = address;
	this->port = port;
}

HttpResponse HttpClient::sendRequest(std::string filePath)
{
	std::string filename = FileSystem::Path::getName(filePath);
	this->connector.connect(address, port);
	HttpRequest request;
	request.Type = "PUT";
	request.Resource = filename;
	request.ContentType = "file";
	return HttpResponse();
}
