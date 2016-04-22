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
	d.Parse(metadataString.c_str());
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
	auto filePath = modulePath + "\metadata.json";
	FileSystem::File metadataFile(filePath);
	std::string metadataString = metadataFile.readAll();
	return RepositoryMetadataHelper::Deserialize(metadataString);
}