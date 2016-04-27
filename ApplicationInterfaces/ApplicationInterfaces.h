// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the APPLICATIONINTERFACES_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// APPLICATIONINTERFACES_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef APPLICATIONINTERFACES_EXPORTS
#define APPLICATIONINTERFACES_API __declspec(dllexport)
#else
#define APPLICATIONINTERFACES_API __declspec(dllimport)
#endif
#include <string>
using namespace std;
// This class is exported from the ApplicationInterfaces.dll
class APPLICATIONINTERFACES_API CApplicationInterfaces {
public:
	CApplicationInterfaces(void);
	// TODO: add your methods here.
};

// Send request to server and get the result.
// requestString is the json format string of HttpRequest Object
// Later on I may change the HttpBase package to CLR library thus both WPF and C++ project can use them directly
extern "C"  APPLICATIONINTERFACES_API void sendRequest(const char *requestString, char *result, int length);


// Send request to server and get the result.
// requestString is the json format string of HttpRequest Object
// Later on I may change the HttpBase package to CLR library thus both WPF and C++ project can use them directly
extern "C"  APPLICATIONINTERFACES_API void getFile(const char *requestString, char *result, int length);