// ApplicationInterfaces.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "ApplicationInterfaces.h"
#include "../HttpBase/HttpPacket.h"
#include "../HttpClient/HttpClient.h"
#include "../HttpBase/HttpUtils.h"
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#include "../HttpServer/RepositoryMetadata.h"

using namespace rapidjson;
using namespace std;

// This is an example of an exported function.
APPLICATIONINTERFACES_API void sendRequest(const char *requestString, char *result, int length)
{	
	auto request = HttpUtils::deserializeRequest(std::string(requestString));
	HttpClient client;
	client.setConnection("localhost", 9080);
	vector<string> files;
	for (auto f : request.files) {
		files.push_back(f.first);
	}
	auto response = client.sendRequest(request.Resource, request.FormData, files);
	auto ss = std::string(response.contentString);

	int i = 0;
	for (auto c : ss) {
		result[i++] = c;
	}
	result[ss.size()] = 0;
}

APPLICATIONINTERFACES_API void getFile(const char * requestString, char * result, int length)
{
	auto request = HttpUtils::deserializeRequest(std::string(requestString));
	HttpClient client;
	client.setConnection("localhost", 9080);
	vector<string> files;
	for (auto f : request.files) {
		files.push_back(f.first);
	}
	string filepath = client.GetFile(request.Resource, request.FormData, files);
	int i = 0;
	for (auto c : filepath) {
		result[i++] = c;
	}
	result[filepath.size()] = 0;
}

// This is the constructor of a class that has been exported.
// see ApplicationInterfaces.h for the class definition
CApplicationInterfaces::CApplicationInterfaces()
{
    return;
}
