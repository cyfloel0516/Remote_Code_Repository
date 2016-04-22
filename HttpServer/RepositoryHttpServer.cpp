/*
* RepositoryHttpServer.cpp
* This file is the implementation of RepositoryHttpServer.h
*
* Author: Junkai Yang
*/

#include "RepositoryHttpServer.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>
#include <functional>
using namespace std;

const string RepositoryHttpServer::repository_path = FileSystem::Path::getFullFileSpec("../Code_Repository/");

void RepositoryHttpServer::CheckRepositoryExist(){
	auto exist = FileSystem::Directory::exists(RepositoryHttpServer::repository_path);
	if (!exist)
		FileSystem::Directory::create(RepositoryHttpServer::repository_path);
}

HttpResponse RepositoryHttpServer::FilesCheckIn(HttpRequest request)
{
	RepositoryHttpServer::CheckRepositoryExist();
	vector<string> fileList;
	string folderPath = RepositoryHttpServer::repository_path + request.FormData["ModuleName"] + "_" + RepositoryHttpServer::CurrentDatetimeString() + "/";
	FileSystem::Directory::create(folderPath);
	for (auto it = request.files.begin(); it != request.files.end(); it++) {
		auto fileName = it->first;
		auto fileStream = it->second;
		auto filePath = folderPath + fileName;
		fileList.push_back(filePath);
		std::string fileContent = fileStream->str();
		FileSystem::File file(filePath);
		file.open(FileSystem::File::out, FileSystem::File::binary);
		file.putBuffer(fileContent.size(), &fileContent[0]);
		file.close();
	}
	return HttpResponse();
}

string RepositoryHttpServer::CurrentDatetimeString()
{
	char buffer[15];
	time_t t = time(0);   // get time now
	struct tm now;
	localtime_s(&now, &t);
	auto timeString = strftime(buffer, 15, "%Y%m%d%H%M%S", &now);
	return std::string(buffer, 14);
}


//----< test stub starts here >----------------------------------------------

int main()
{
	try
	{
		SocketSystem ss;
		SocketListener sl(9080, Socket::IP4);
		HttpRequestHandler cp;
		std::function<HttpResponse(HttpRequest)> handler = [](HttpRequest request){
			RepositoryHttpServer handlerClass;
			return handlerClass.FilesCheckIn(request);
		};
		cp.addRoute("/repository/checkin", handler);
		sl.start(cp);

		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{

	}
}

