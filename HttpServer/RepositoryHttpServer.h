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
#include "RepositoryMetadata.h"
#include <string>

using namespace std;


// This class includes the function to handle HTTP request from client
class RepositoryHttpServer {
public:
	// Check if the repository is exist, if not, then create it.
	void CheckRepositoryExist();

	// Get current opend module folder, if no opend folder exist then create a new version and return it.
	RepositoryMetadata GetCurrentOpenedModule(string moduleName);

	// Module or package check-in function, it corresponds to the route: "/repository/checkin"
	HttpResponse FilesCheckIn(HttpRequest request);

	// A handler to list all modules in repository, it corresponds to the route: "/repository/list"
	HttpResponse ListRepository(HttpRequest request);
	
	// A handler to set the closed of metadata, it corresponds to the route: "/repository/close_module"
	HttpResponse CloseModule(HttpRequest request);

	// A handler to set the closed of metadata, it corresponds to the route: "/repository/close_module"
	HttpResponse DownloadModule(HttpRequest request);
	
	// Get version from folder path
	std::string GetVersionFromPath(std::string path);

	// Regenerate the dependency relation
	static void GenerateDependencyRelation();
		
	// A helper function to get current datetime string, the format is YYYYMMDD_HHMMSS
	static string CurrentDatetimeString();


private:
	const static string repository_path;
};