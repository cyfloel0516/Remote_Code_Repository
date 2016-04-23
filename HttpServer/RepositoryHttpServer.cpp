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
#include <algorithm>
#include "../FileSystem_Windows/FileSystemSearchHelper.h"
#include "RepositoryMetadata.h"
#include "RepositoryDependencyAnalyser.h"


using namespace std;

const string RepositoryHttpServer::repository_path = FileSystem::Path::getFullFileSpec("../Code_Repository/");

void RepositoryHttpServer::CheckRepositoryExist(){
	auto exist = FileSystem::Directory::exists(RepositoryHttpServer::repository_path);
	if (!exist)
		FileSystem::Directory::create(RepositoryHttpServer::repository_path);
}

std::string RepositoryHttpServer::GetCurrentOpenedModule(string moduleName)
{
	auto moduleFolders = FileSystemSearchHelper::searchDirectories(RepositoryHttpServer::repository_path, moduleName + "_*");
	for (auto folderPath : moduleFolders) {
		auto metaDataFile = folderPath + "/" + "metadata.json";
	}
	return std::string();
}

HttpResponse RepositoryHttpServer::FilesCheckIn(HttpRequest request)
{
	RepositoryHttpServer::CheckRepositoryExist();
	vector<string> fileList;
	bool closed = request.FormData["Closed"] == "True";
	auto checkInVersion = RepositoryHttpServer::CurrentDatetimeString();
	auto moduleName = request.FormData["ModuleName"] + "__" + checkInVersion;
	string folderPath = RepositoryHttpServer::repository_path + moduleName + "/";
	
	FileSystem::Directory::create(folderPath);
	RepositoryMetadata metadata(request.FormData["ModuleName"], checkInVersion, closed);
	// Store files to module folder.
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
		metadata.FileList.push_back(fileName);
	}
	// Save metadata to the folder

	
	RepositoryMetadataHelper::SaveMetadata(folderPath, metadata);
	
	// Regenerate dependency relation
	if (metadata.Closed) {
		RepositoryHttpServer::GenerateDependencyRelation();
	}
	return HttpResponse();
}

void RepositoryHttpServer::GenerateDependencyRelation(){
	RepositoryDependencyAnalyser analyser;
	analyser.InitTypeTable();
	// go through each folder and see if the repository is closed, if yes, then take all files into the list for analysis
	auto directories = FileSystem::Directory::getDirectories(RepositoryMetadataHelper::repository_path, "*");
	std::for_each(directories.begin(), directories.end(), [](string &s) { s = RepositoryMetadataHelper::repository_path + s;});
	for (auto dir : directories) {
		auto metadata = RepositoryMetadataHelper::GetMetadata(dir);
		if (!metadata.Name.empty() && metadata.Closed) {
			// Do dependency analysis
			auto dependencies = analyser.GetDependency(dir);
			for (auto dep : dependencies) {
				auto it = std::find_if(metadata.Dependencies.begin(), metadata.Dependencies.end(), [&dep](const string&v) {
					return RepositoryMetadata::VersionCompared(v, dep);
				});
				if (it == metadata.Dependencies.end())
					metadata.Dependencies.push_back(dep);
				else
					*it = dep;
			}
			RepositoryMetadataHelper::SaveMetadata(RepositoryMetadataHelper::repository_path + metadata.getFullName() + "/" , metadata);
		}
	}
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
	// Test Repository Metadata:
	
	RepositoryMetadata metadata;
	metadata.Name = "TestModule";
	metadata.Version = "1.0.0";
	metadata.Closed = false;
	metadata.Dependencies = vector<string>{ "Module1", "Module2" };
	metadata.FileList = vector<string>{ "File1", "File2", "File3" };
	auto jsonString = RepositoryMetadataHelper::Serialize(metadata);

	auto metadata2 = RepositoryMetadataHelper::Deserialize(jsonString);
	
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

