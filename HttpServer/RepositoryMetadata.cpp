#pragma once
/*
*	RepositoryMetadata.cpp: Implementation of RepositoryMetadata.h
*	This package includes the definition of the code package metadata as well as some help function
*
*	Author: Junkai Yang
*/
#include "RepositoryMetadata.h"
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#include <algorithm>
using namespace rapidjson;

const string RepositoryMetadataHelper::repository_path = FileSystem::Path::getFullFileSpec("../Code_Repository/");

std::string RepositoryMetadataHelper::Serialize(RepositoryMetadata metadata)
{
	Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	d.AddMember("Name", StringRef(metadata.Name.c_str()), allocator);
	d.AddMember("Version", StringRef(metadata.Version.c_str()), allocator);
	d.AddMember("Closed", metadata.Closed, allocator);

	//Add dependency array to json object
	rapidjson::Value dependencyArray(rapidjson::kArrayType);
	for (auto dependency : metadata.Dependencies) {
		dependencyArray.PushBack(Value(dependency.c_str(), allocator), d.GetAllocator());
	}
	d.AddMember("Dependencies", dependencyArray, d.GetAllocator());

	// Add file list array to json object
	rapidjson::Value fileListArray(rapidjson::kArrayType);
	for (auto file : metadata.FileList) {
		fileListArray.PushBack(Value(file.c_str(), allocator), d.GetAllocator());
	}
	d.AddMember("FileList", fileListArray, d.GetAllocator());

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);
	const char* charString = buffer.GetString();
	return std::string(charString);
}

RepositoryMetadata RepositoryMetadataHelper::Deserialize(string metadataString)
{
	Document d;
	d.Parse(StringRef(metadataString.c_str()));
	RepositoryMetadata metadata;
	metadata.Name = d["Name"].GetString();
	metadata.Version = d["Version"].GetString();
	metadata.Closed = d["Closed"].GetBool();

	// Get dependencies and file list
	const Value& dependencies = d["Dependencies"];
	for (SizeType i = 0; i < dependencies.Size(); i++) {
		metadata.Dependencies.push_back(dependencies[i].GetString());
	}

	const Value& files = d["FileList"];
	for (SizeType i = 0; i < files.Size(); i++) {
		metadata.FileList.push_back(files[i].GetString());
	}

	return metadata;
}

void RepositoryMetadataHelper::SaveMetadata(string modulePath, RepositoryMetadata metadata) {
	auto filePath = modulePath + "\metadata.json";
	if (FileSystem::File::exists(filePath))
		FileSystem::File::remove(filePath);
	FileSystem::File metadataFile(filePath);
	auto metadataContent = RepositoryMetadataHelper::Serialize(metadata);
	metadataFile.open(FileSystem::File::out, FileSystem::File::binary);
	metadataFile.putBuffer(metadataContent.size(), &metadataContent[0]);
}

RepositoryMetadata RepositoryMetadataHelper::GetMetadata(string modulePath)
{
	auto filePath = modulePath + "/metadata.json";
	if (FileSystem::File::exists(filePath)) {
		FileSystem::File metadataFile(filePath);
		metadataFile.open(FileSystem::File::in);
		std::string metadataString = metadataFile.readAll();
		metadataFile.close();
		return RepositoryMetadataHelper::Deserialize(metadataString);
	}
	else {
		return RepositoryMetadata();
	}

}

string RepositoryMetadataHelper::Serialize(vector<RepositoryMetadata> metadatas)
{
	Document result;
	result.SetArray();
	rapidjson::Document::AllocatorType& allocator = result.GetAllocator();

	for (auto i = 0; i < metadatas.size(); i++) {
		auto metadata = metadatas[i];
		rapidjson::Value d(rapidjson::kObjectType); 
		d.SetObject();
		d.AddMember("Name", Value(metadata.Name.c_str(), allocator).Move(), allocator);
		d.AddMember("Version", Value(metadata.Version.c_str(), allocator).Move(), allocator);
		d.AddMember("Closed", metadata.Closed, allocator);

		//Add dependency array to json object
		rapidjson::Value dependencyArray(rapidjson::kArrayType);
		for (auto dependency : metadata.Dependencies) {
			dependencyArray.PushBack(Value(dependency.c_str(), allocator), allocator);
		}
		d.AddMember("Dependencies", dependencyArray, allocator);

		// Add file list array to json object
		rapidjson::Value fileListArray(rapidjson::kArrayType);
		for (auto file : metadata.FileList) {
			fileListArray.PushBack(Value(file.c_str(), allocator), allocator);
		}
		d.AddMember("FileList", fileListArray, allocator);

		result.PushBack(d, allocator);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	result.Accept(writer);
	const char* charString = buffer.GetString();
	return std::string(charString);
}

RepositoryMetadata::RepositoryMetadata(){}

RepositoryMetadata::RepositoryMetadata(string name, string version, bool closed){
	this->Name = name;
	this->Version = version;
	this->Closed = closed;
}

std::string RepositoryMetadata::getFullName()
{
	return this->Name + "__" + this->Version;
}

bool RepositoryMetadata::VersionCompared(string version1, string version2)
{
	auto index1 = version1.find_last_of("__");
	auto index2 = version2.find_last_of("__");
	if (index1 == index2 && version1.substr(0, index1) == version2.substr(0, index2))
		return true;
	else
		return false;
}
