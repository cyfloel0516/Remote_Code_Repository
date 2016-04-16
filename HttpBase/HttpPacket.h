#pragma once
/*
*	HttpPacket.h
*	This package define some http packet class, including the httprequest and httpresponse object
*
*	Author: Junkai Yang
*/
#include <string>


struct HttpPacket{
	std::string Type;
	std::string Resource;
	int StatusCode;
	std::string Protocol;
	std::string StatusText;
	std::string ContentType;
	int ContentLength;
};


struct HttpResponse : HttpPacket {  };

struct HttpRequest : HttpPacket {  
	std::string boundry;
	std::vector<std::string> files;
};