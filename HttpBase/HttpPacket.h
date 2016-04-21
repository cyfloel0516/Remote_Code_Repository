#pragma once
/*
*	HttpPacket.h
*	This package define some http packet class, including the httprequest and httpresponse object
*
*	Author: Junkai Yang
*/
#include <map>
#include <string>


struct HttpPacket{
	std::string Type;
	std::string Resource;
	int StatusCode;
	std::string Protocol;
	std::string StatusText;
	std::string ContentType;
	std::string contentString;
	int ContentLength;
};

struct HttpRequestLine {
	std::string Name;
	std::string Value;
	std::map<std::string, std::string> Proterties;
};

struct HttpResponse : HttpPacket {};

struct HttpRequest : HttpPacket {  
	std::string boundary;
	std::map<std::string, std::ostream*> files;
};