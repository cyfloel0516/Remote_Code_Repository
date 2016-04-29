/*
*	RepositoryDependencyAnalyser.cpp
*	This file is the implemantation of RepositoryDependencyAnalyser.h
*
*	Author: Junkai Yang
*/
#include "RepositoryDependencyAnalyser.h"
#include <map>
#include <vector>
#include <string>

void RepositoryDependencyAnalyser::InitTypeTable(string moduleName){
	// go through each folder and see if the repository is closed, if yes, then take all files into the list for analysis
	vector<string> fileList;
	auto directories = FileSystemSearchHelper::searchDirectories(RepositoryMetadataHelper::repository_path, "*");
	map<string, RepositoryMetadata> modules;
	for (auto dir : directories) {
		auto metadata = RepositoryMetadataHelper::GetMetadata(dir);
		auto it = modules.find(metadata.Name);
		if (it != modules.end()) {
			if (it->second.Version < metadata.Version)
				it->second = metadata;
		}
		else
			modules.insert({ metadata.Name, metadata });
	}

	for(auto module : modules){
		auto dir = RepositoryMetadataHelper::repository_path + module.second.getFullName() + "/";
		auto metadata = RepositoryMetadataHelper::GetMetadata(dir);
		// Only analysis the closed repository
		if (!metadata.Name.empty() && metadata.Closed && !RepositoryMetadata::VersionCompared(metadata.getFullName(), moduleName) ) {
			for (auto f : metadata.FileList) {
				auto filePath = RepositoryMetadataHelper::repository_path + "/" + metadata.getFullName() + "/" + f;
				fileList.push_back(filePath);
				moduleLookupTable.insert({ filePath, metadata.getFullName() });
			}
		}
	}
	ThreadPool pool(5);
	this->typeTable = test_getType(pool, fileList);
}

vector<string> RepositoryDependencyAnalyser::GetDependency(string modulePath)
{
	ThreadPool pool(5);
	auto metadata = RepositoryMetadataHelper::GetMetadata(modulePath);
	vector<string> fileList;
	vector<string> result;
	for (auto file : metadata.FileList) {
		fileList.push_back(RepositoryMetadataHelper::repository_path + "/" + metadata.getFullName() + "/" + file);
	}
	auto dependencies = test_getDependency(pool, fileList, &this->typeTable);
	for (size_t i = 0; i < dependencies.Size(); i++) {
		auto moduleIt = this->moduleLookupTable.find(dependencies[i].toFile);
		if (moduleIt != this->moduleLookupTable.end() && moduleIt->second != metadata.getFullName() 
			&& !RepositoryMetadata::VersionCompared(moduleIt->second, metadata.getFullName())) {
			result.push_back(moduleIt->second);
		}
	}
	return result;
}

#ifdef TEST_ANALYSER
void main() {
	RepositoryDependencyAnalyser analyser;
	cout << "Start generate type table" << endl;
	analyser.InitTypeTable();
	cout << "End generate type table" << endl;
	return;
}
#endif