#pragma once
#include "../Sockets/Sockets.h"
#include "../HttpBase/HttpPacket.h"
#include "../FileSystem_Windows/FileSystem.h"

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
	
	HttpResponse deserializeResponse(string responseString);
private:
	SocketConnecter connector;
	std::string address;
	int port;
};