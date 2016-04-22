#pragma once
/*
*	RepositoryHttpServer.h
*	This package use the facility of HTTP server and implement some request handler function.
*	It will create a http server and register some route handler then handle the incoming request from client.
*
*	Author: Junkai Yang
*/

#include "HttpServer.h"
#include "../HttpBase/HttpPacket.h"
#include "../HttpBase/HttpUtils.h"
#include "../FileSystem_Windows/FileSystem.h"

#include <string>

using namespace std;


// This class includes the function to handle HTTP request from client
class RepositoryHttpServer {
public:
	// Check if the repository is exist, if not, then create it.
	void CheckRepositoryExist();

	// Get current opend module folder, if no opend folder exist then create a new version and return it.
	std::string GetCurrentOpenedModule(string moduleName);

	// Module or package check-in function
	HttpResponse FilesCheckIn(HttpRequest request);

	// A helper function to get current datetime string, the format is YYYYMMDD_HHMMSS
	static string CurrentDatetimeString();
private:
	const static string repository_path;
};