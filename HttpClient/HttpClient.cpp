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
	SocketSystem ss;
	while (!this->connector.connect(address, port))
	{
		::Sleep(100);
	}
	HttpRequest request;
	request.Type = "PUT";
	request.ContentType = "files";
	
	for (auto filePath : filePaths) {
		request.files.insert({ filePath, nullptr });
	}
	request.boundary = "--------------------------JukaiYangCIS687" + std::to_string(rand() % 1000);
	auto s = HttpUtils::serialize(request);
	this->connector.sendString(s);
	this->connector.shutDownSend();
	this->connector.recvString('\n');
	return HttpResponse();
}


//
//int main()
//{
//
//	try
//	{
//		std::vector<std::string> files{ "./HttpClient.cpp", "./HttpClient.h" };
//		HttpClient client;
//		client.setConnection("localhost", 9080);
//		client.sendRequest(files);
//	}
//	catch (std::exception& ex)
//	{
//		std::cout << ex.what();
//	}
//}
