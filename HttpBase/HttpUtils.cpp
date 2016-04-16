/*
*	HttpUtils.h
*	Implementation of HttpUtils.h
*
*	Author: Junkai Yang
*/

#include "HttpUtils.h"
#include "../Utilities/Utilities.h"
#include "../FileSystem_Windows/FileSystem.h"
#include <vector>
#include <iostream>

std::string HttpUtils::getCommand(std::string line) {
	int pos = line.find(':');
	std::string command;
	if (pos != std::string::npos) {
		command = Utilities::StringHelper::trim(line.substr(0, pos));
	}
	return command;
}

std::string HttpUtils::getValue(std::string line) {
	int pos = line.find(':');
	std::string value;
	if (pos != std::string::npos) {
		value = Utilities::StringHelper::trim(line.substr(pos));
	}
	return value;
}

std::string HttpUtils::serialize(HttpRequest request) {
	
	std::string result;
	int requestSize = 1000, i = 0, j = 0;
	bool isFile = false;
	if (request.files.size() != 0) {
		for (i = 0; i < request.files.size(); i++) {
			FileSystem::FileInfo fi(request.files[i]);
			requestSize += fi.size();
		}
		result.reserve(requestSize);
		isFile = true;
	}
	else {
		result.reserve(requestSize);
	}
	
	std::string tempResult = request.Type + " " + request.Resource + " " + request.Protocol + "\n";

	tempResult += "Content-Type: " + request.ContentType + "; boundry=" + request.boundry + "\n"
		+ "Content-Length: 0" + "\n";

	for (i = 0; i < tempResult.size(); i++) result.push_back(tempResult[i]);

	// Serialize file content
	if (isFile) {
		result += "\n" + request.boundry + "\n";
		for (i = 0; i < request.files.size(); i++) {
			auto filename = FileSystem::Path::getName(request.files[i]);
			auto contentHeader = "Content-Disposition: form-data; filename=\"" + filename + "\"\n\n";
			result += contentHeader;
			auto fileContent = HttpUtils::getFileContent(request.files[i]);
			for (j = 0; j < fileContent.size(); j++) result.push_back(fileContent[j]);
			result.push_back('\n');
			result += request.boundry + "\n";
		}
	}

	return result;
}

std::vector<char> HttpUtils::getFileContent(std::string filename) {
	std::vector<char> result;
	FileSystem::FileInfo fileInfo(filename);
	FileSystem::File file(filename);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	result.reserve(fileInfo.size());
	char buffer[1024];
	while (true) {
		int len = file.getBuffer(1024, buffer);
		for (int i = 0; i < len; i++) result.push_back(buffer[i]);
		if (len < 1024) {
			file.close();
			break;
		}
	}
	return result;
}

void HttpUtils::putStringToVector(std::vector<char>& destination, std::string value) {
	for (int i = 0; i < value.size(); i++) {
		destination.push_back(value[i]);
	}
}


#ifdef TEST_HTTPUTILS
int main() {
	HttpRequest request;
	request.boundry = "---------------12345";
	request.files.push_back(FileSystem::Path::getFullFileSpec("./HttpUtils.cpp"));
	request.Type = "POST";
	request.Resource = "test_resource";

	auto result = HttpUtils::serialize(request);

	std::string s;
	for (auto i = 0; i < result.size(); i++) {
		s += result[i];
	}
	std::cout << s;
}
#endif