/*
*	HttpClient.h
*	This file is a simple http client class
*	It includes a send request function and some usefule function to handle response
*
*	Author: Junkai Yang
*/
#pragma once
#include "../Sockets/Sockets.h"
#include "../HttpBase/HttpPacket.h"
#include "../FileSystem_Windows/FileSystem.h"
#include <string>

using namespace std;

using namespace std;
class HttpClient {
public:
	// Default Constructor Function

	// Deconstructor function
	~HttpClient();

	// Connect to a server
	void setConnection(std::string address, int port);
	
	// Send file through HTTP
	HttpResponse sendRequest(string resource, map<string, string> formData, vector<string> filePaths);
	
	// This function is to get a file from server
	// Because send request function use text transfer so I can only create a new function to 
	// transfer binary data
	string GetFile(string resource, map<string, string> formData, vector<string> filePaths);
	
	// Deserialize response string to response object
	HttpResponse deserializeResponse(string responseString);
private:
	SocketConnecter connector;
	std::string address;
	int port;
};