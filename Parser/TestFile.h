// This namespace and all of its elements are used for testing only.
#include <iostream>
#include <string>

using namespace std;
namespace NamespaceForTest {
	class testClass {
	public:
		void testFunc() {
			auto lambdaFunc = [&]() {
				int b = 1;
				do {
					b++;
				} while (b < 10);

				while (b < 20) {
					b++;
				}
				return 12345;
			};
			try {
				int a = 1;
				if (a == 1) {
					std::cout << 1;
				}
				else if (a == 2) {
					std::cout << 2;
				}
				else {
					std::cout << 3;
				}
				switch (a) {
				case 1: {
					a = 2;
					break;
				};
				default:
					a = 3;
				}
			}
			catch (const std::exception& ex) {
				std::cout << ex.what();
			}
		};
	};

	struct testStruct {
		int testProp1 = 1;
		std::string testProp2 = "only for test";
	};
}