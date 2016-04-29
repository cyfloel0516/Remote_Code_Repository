#pragma once
/*
*	RepositoryMetadata.h
*	This package includes the definition of the code package metadata as well as some help function
*
*	Author: Junkai Yang
*/

#include "../FileSystem_Windows/FileSystem.h"
#include <string>
#include <vector>

using namespace std;

// This is the repository metadata structure, it present a module.
struct RepositoryMetadata {
	string Name;
	string Version;
	bool Closed;
	vector<string> Dependencies;
	vector<string> FileList;
	
	RepositoryMetadata();
	RepositoryMetadata(string name, string version, bool closed);

	// Get the full name of a module, it combine its name and version
	std::string getFullName();
	
	// Compare if two module is same or not
	static bool VersionCompared(string version1, string version2);
};

// This class is a helper class contains some heler function to generate or get repository metadata json or object
class RepositoryMetadataHelper {
public:
	// Serialize a metadata object to json string
	static std::string Serialize(RepositoryMetadata metadata);

	// Deserialize a json string to a repository metadata object
	static RepositoryMetadata Deserialize(string metadataString);

	// Save metadata to file
	static void SaveMetadata(string modulePath, RepositoryMetadata metadata);

	// Get metadata from file
	static RepositoryMetadata GetMetadata(string modulePath);

	// This is the static variable to store the path of the code repository
	const static string repository_path;

	// Serialize a metadata to string
	static string Serialize(vector<RepositoryMetadata> metadatas);
};
