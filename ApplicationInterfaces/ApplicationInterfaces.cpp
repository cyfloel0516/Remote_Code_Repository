// ApplicationInterfaces.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include "ApplicationInterfaces.h"
#include "../HttpClient/HttpClient.h"


// This is an example of an exported variable
APPLICATIONINTERFACES_API int nApplicationInterfaces=0;

// This is an example of an exported function.
APPLICATIONINTERFACES_API int fnApplicationInterfaces()
{
	try
	{
		std::vector<std::string> files{"C://Users//cyflo//workspace//CIS687//project4_remote_code_repository//remote_code_repository//HttpClient//HttpClient.cpp"};
		HttpClient client;
		client.setConnection("localhost", 9080);
		client.sendRequest(files);
	}
	catch (std::exception& ex)
	{
		return 2;
	}
	return 1;
}

// This is the constructor of a class that has been exported.
// see ApplicationInterfaces.h for the class definition
CApplicationInterfaces::CApplicationInterfaces()
{
    return;
}
