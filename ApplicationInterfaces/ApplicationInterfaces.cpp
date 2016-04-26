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

		/*map<string, string> formData;
		formData.insert({ "ModuleName", "HttpClient" });
		formData.insert({ "Closed", "True" });
		std::vector<std::string> files{ "C:/Users/cyflo/workspace/CIS687/project4_remote_code_repository/remote_code_repository/HttpClient/HttpClient.cpp", "C:/Users/cyflo/workspace/CIS687/project4_remote_code_repository/remote_code_repository/HttpClient/HttpClient.h" };
		HttpClient client;
		client.setConnection("localhost", 9080);
		client.sendRequest("/repository/checkin", formData, files);

		map<string, string> formData1;
		formData1.insert({ "ModuleName", "Sockets" });
		formData1.insert({ "Closed", "True" });
		std::vector<std::string> files1{ "C:/Users/cyflo/workspace/CIS687/project4_remote_code_repository/remote_code_repository/Sockets/Sockets.cpp", "C:/Users/cyflo/workspace/CIS687/project4_remote_code_repository/remote_code_repository/Sockets/Sockets.h" };
		HttpClient client1;
		client1.setConnection("localhost", 9080);
		client1.sendRequest("/repository/checkin", formData1, files1);*/
		cout << "testtest" << endl;
		auto request = HttpUtils::deserializeRequest(std::string(requestString));
		HttpClient client;
		client.setConnection("localhost", 9080);
		vector<string> files;
		for (auto f : request.files) {
			files.push_back(f.first);
		}
		auto response = client.sendRequest(request.Resource, request.FormData, files);
		//auto ss = std::string(HttpUtils::serialize(response));
		auto ss = std::string(response.contentString);

		cout << "testtest1123" << ss << endl;
		int i = 0;
		for (auto c : ss) {
			result[i] = c;
			i++;
		}
		result[ss.size() - 1] = 0;
	/*
	HttpRequest request;
	Document d;
	d.Parse(StringRef(requestString));
	auto formData = d["formData"].GetArray();
	for (SizeType i = 0; i < formData.Size(); i++) {
		auto key = formData[i]["key"].GetString();
		auto value = formData[i]["value"].GetString();
		request.FormData.insert({ key, value });
	}

	auto files = d["files"].GetArray();
	for (SizeType i = 0; i < files.Size(); i++) {
		auto path = files[i].GetString();
		request.files.insert({ path, nullptr });
	}

	request.Resource = d["resource"].GetString();
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);
	const char* charString = buffer.GetString();
	auto ss = std::string(charString);
	for (auto i = 0; i < ss.size(); i++)
		result[i] = ss[i];*/
}

// This is the constructor of a class that has been exported.
// see ApplicationInterfaces.h for the class definition
CApplicationInterfaces::CApplicationInterfaces()
{
    return;
}
