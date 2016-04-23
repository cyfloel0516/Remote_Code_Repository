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

struct RepositoryMetadata {
	string Name;
	string Version;
	bool Closed;
	vector<string> Dependencies;
	vector<string> FileList;
	
	RepositoryMetadata();

	RepositoryMetadata(string name, string version, bool closed);

	std::string getFullName();

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

	const static string repository_path;
};
