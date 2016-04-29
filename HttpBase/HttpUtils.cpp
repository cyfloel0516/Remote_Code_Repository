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
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#include <algorithm>
#include <iostream>
using namespace std;
using namespace rapidjson;

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
		value = Utilities::StringHelper::trim(line.substr(pos + 1));
	}
	return value;
}

std::string HttpUtils::serialize(HttpRequest request, bool withfile) {
	std::string result;
	int requestSize = 1000;
	size_t i = 0, j = 0;
	bool isFile = false;
	if (request.files.size() != 0 && withfile) {
		for (auto it = request.files.begin(); it != request.files.end(); it++) {
			FileSystem::FileInfo fi(it->first);
			requestSize += fi.size();
		}
		result.reserve(requestSize);
		isFile = true;
	}
	else {
		result.reserve(requestSize);
	}
	
	std::string tempResult = request.Type + " " + request.Resource + " " + request.Protocol + "\n";

	tempResult += "Content-Type: " + request.ContentType + "; boundary=" + request.boundary + "\n"
		+ "Content-Length: 0" + "\n";

	for (i = 0; i < tempResult.size(); i++) result.push_back(tempResult[i]);
	
	// Serialize form data
	for (auto it = request.FormData.begin(); it != request.FormData.end(); it++) {
		result += "\n" + request.boundary + "\n";
		result += "Content-Disposition: form-data; name=" + it->first + "\n\n";
		result += it->second + "\n" + request.boundary + "\n";
	}

	// Serialize file content
	if (isFile && withfile) {
		result += "\n" + request.boundary + "\n";
		for (auto it = request.files.begin(); it != request.files.end(); it++) {
			auto filename = FileSystem::Path::getName(it->first);
			auto contentHeader = "Content-Disposition: form-data; filename=" + filename + "\n\n";
			result += contentHeader;
			auto fileContent = HttpUtils::getFileContent(it->first);
			for (j = 0; j < fileContent.size(); j++) result.push_back(fileContent[j]);
			result.push_back('\n');
			result += request.boundary + "\n";
		}
	}

	return result;
}

std::string HttpUtils::serialize(HttpResponse response)
{
	std::string result;
	if (response.ContentType != "file") {
		result = response.Protocol + " " + std::to_string(response.StatusCode) + " " + response.StatusText + "\n";
		result += "Content-Type: " + response.ContentType + "\n";
		result += "Content-Length: " + std::to_string(response.contentString.size()) + "\n";
		result += "\n";
		for (auto c : response.contentString) {
			result += c;
		}
	}
	else {
		return response.contentString;
	}
	return result;
}

HttpRequestLine HttpUtils::getLineData(std::string line) {
	HttpRequestLine lineData;
	std::map<std::string, std::string> result;
	if (line.find(";") != std::string::npos) {
		auto keyValues = Utilities::StringHelper::split(line, ';');
		lineData.Name = HttpUtils::getCommand(keyValues[0]);
		lineData.Value = HttpUtils::getValue(keyValues[0]);
		for (size_t i = 1; i < keyValues.size(); i++) {
			auto keyValue = keyValues[i];
			auto pair = Utilities::StringHelper::split(keyValue, '=');
			lineData.Proterties.insert({ Utilities::StringHelper::trim(pair[0]), Utilities::StringHelper::trim(pair[1]) });
		}
	}
	else {
		lineData.Name = HttpUtils::getCommand(line);
		lineData.Value = HttpUtils::getValue(line);
	}
	return lineData;
}

HttpRequest HttpUtils::deserializeRequest(std::string str)
{	
	cout << str << endl;
	HttpRequest request;
	Document d;
	d.Parse(StringRef(str.c_str()));
	request.Resource = d["Resource"].GetString();

	// Get dependencies and file list
	const Value& formData = d["FormData"];
	for (SizeType i = 0; i < formData.Size(); i++) {
		request.FormData.insert({ formData[i]["key"].GetString(), formData[i]["value"].GetString() });
	}

	const Value& files = d["Files"];
	for (SizeType i = 0; i < files.Size(); i++) {
		request.files.insert({ files[i].GetString(), nullptr });
	}

	return request;
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
	for (size_t i = 0; i < value.size(); i++) {
		destination.push_back(value[i]);
	}
}


#ifdef TEST_HTTPUTILS
int main() {
	HttpRequest request;
	request.boundary = "---------------12345";
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