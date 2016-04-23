#include <iostream>
#include <string>
#include "ScopeStack.h"

//----< test stub >--------------------------------------------
#ifdef SCOPESTACK_TEST
using namespace std;
int main() {
	auto scopeStack = ScopeStack<std::string>();
	scopeStack.push("123");
	scopeStack.push("abc");

	auto topS = scopeStack.top();
	std::cout << topS << std::endl;
	
	scopeStack.pop();
	std::cout << scopeStack.size() << endl;
	return 1;
}
#endif