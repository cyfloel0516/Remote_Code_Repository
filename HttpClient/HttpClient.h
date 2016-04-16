#pragma once
#include "../Sockets/Sockets.h"
#include "../HttpBase/HttpPacket.h"
#include "../FileSystem_Windows/FileSystem.h"
class HttpClient {
public:
	// Default Constructor Function

	// Deconstructor function
	~HttpClient();

	// Connect to a server
	void setConnection(std::string address, int port);
	
	// Send file through HTTP
	HttpResponse sendRequest(std::string filePath);

private:
	SocketConnecter connector;
	std::string address;
	int port;
};