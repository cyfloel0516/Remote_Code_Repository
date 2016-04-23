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

HttpResponse HttpClient::sendRequest(string resource, map<string, string> formData, vector<string> filePaths)
{
	SocketSystem ss;
	while (!this->connector.connect(address, port)){
		::Sleep(100);
	}
	HttpRequest request;
	request.Type = "PUT";
	request.ContentType = "multipart/form-data";
	//request.Resource = "/repository/checkin";
	request.Resource = resource;
	for (auto it = formData.begin(); it != formData.end(); it++)
		request.FormData.insert({ it->first, it->second });
		//request.FormData.insert({ "ModuleName", "TestModule" });
	for (auto filePath : filePaths) {
		request.files.insert({ filePath, nullptr });
	}
	request.boundary = "--------------------------JukaiYangCIS687" + std::to_string(rand() % 1000);
	auto s = HttpUtils::serialize(request);
	this->connector.sendString(s);
	this->connector.shutDownSend();
	this->connector.recvString('\n');
	this->connector.shutDown();
	return HttpResponse();
}

int main()
{
	try
	{
		map<string, string> formData;
		formData.insert({ "ModuleName", "HttpClient" });
		formData.insert({ "Closed", "True" });
		std::vector<std::string> files{ "./HttpClient.cpp", "./HttpClient.h" };
		HttpClient client;
		client.setConnection("localhost", 9080);
		client.sendRequest("/repository/checkin", formData, files);

		map<string, string> formData1;
		formData1.insert({ "ModuleName", "Sockets" });
		formData1.insert({ "Closed", "True" });
		std::vector<std::string> files1{ "../Sockets/Sockets.cpp", "../Sockets/Sockets.h" };
		HttpClient client1;
		client1.setConnection("localhost", 9080);
		client1.sendRequest("/repository/checkin", formData1, files1);
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();
	}
}
