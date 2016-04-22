#include "ParallelExecutive.h"

using namespace std;

TypeTable test_getType(ThreadPool& pool, vector<string> files) {
	//cout << "========================================================================" << endl;
	//utils::Title("Requirement (4, 5): Thread Pool and Tasks...");
	//utils::Title("You can see the order of analysis is totally different with the order of enqueue.");
	//utils::Title("Because thread execution is depends on the CPU scheduling.");
	//cout << "Creating thread pool with 5 worker threads..." << endl ;

	std::vector< std::future<TypeTable> > results;
	//cout << "------------------------------------------------------------------------" << endl;
	for (auto file : files) {
		string enqueueMsg = "Job enqueued: type analysis for " + file + "\n";
		//cout << enqueueMsg;
		results.emplace_back(
			pool.enqueue([file]() {
				string startMessage = "Job starts: analysis for file: " + file + "\n";
				cout << startMessage;
				ClassDefinition *classDef = new ClassDefinition();
				TypeDefDefinition *typeDef = new TypeDefDefinition();
				AliasDefinition *aliasDef = new AliasDefinition();
				GlobalFunctionDefinition *gFunctionDef = new GlobalFunctionDefinition();
				TypeAnalysis analyser;
				analyser.AddDetector(classDef);
				analyser.AddDetector(typeDef);
				analyser.AddDetector(aliasDef);
				analyser.AddDetector(gFunctionDef);
				auto result = analyser.Analysis(file);
				string endMessage = "Job finished: analysis for " + file + "\n";
				//cout << endMessage;
				return result;
			})
		);
	}

	TypeTable table;
	for (size_t i = 0; i < results.size(); i++) {
		auto &&result = results[i];
		auto tempResult = result.get();
		table.Merge(tempResult);
	}
	//cout << "========================================================================" << endl;
	return table;
}

DependencyTable test_getDependency(ThreadPool& pool, vector<string> files, TypeTable* typeTable) {
	//cout << "========================================================================" << endl;
	//utils::Title("Requirement (7, 8, 9, 10): Parallel dependency analysis...");
	//utils::Title("I am going to use the same thread pool which is used by type analysis in step 1.");
	//utils::Title("Because my thread pool can accept lambda and return the Future object so it can be reused.");

	std::vector< std::future<DependencyTable> > results;
	//cout << "------------------------------------------------------------------------" << endl;
	for (auto file : files) {
		string enqueueMsg = "Job enqueued: type analysis for " + file + "\n";
		//cout << enqueueMsg;
		results.emplace_back(
			pool.enqueue([file, &typeTable]() {
				string startMessage = "Job starts: analysis for file: " + file + "\n";
				//cout << startMessage;
				DependencyAnalysis dAnalysis;
				InheritDependencyDetector* inheritDetector = new InheritDependencyDetector(typeTable);
				GFunctionDependencyDetector* gFunctionDetector = new GFunctionDependencyDetector(typeTable);
				ClassDependencyDetector* classDetector = new ClassDependencyDetector(typeTable);
				dAnalysis.AddDetector(inheritDetector);
				dAnalysis.AddDetector(gFunctionDetector);
				dAnalysis.AddDetector(classDetector);
				auto result = dAnalysis.Analysis(file);
				string endMessage = "Job finished: analysis for " + file + "\n";
				//cout << endMessage;
				return result;
			})
		);
	}

	DependencyTable table;
	for (size_t i = 0; i < results.size(); i++) {
		auto &&result = results[i];
		auto tempResult = result.get();
		table.Merge(tempResult);
	}
	//cout << "========================================================================" << endl;
	return table;
}
//
//void main(int argc, char* argv[]) {
//	string userInput;
//	if (argc < 3) {
//		cout << "Please input the correct parameters. Usage: " << endl;
//		cout << "ParallelExecutive.exe .. *.h *.cpp" << endl;
//	}
//	ThreadPool pool(5);
//	string root = FileSystem::Path::getFullFileSpec(argv[1]);
//	vector<string> searchPatterns;
//	for (int i = 2; i < argc; i++) {
//		searchPatterns.push_back(argv[i]);
//	}
//	vector<string> files = FileSystemSearchHelper::searchFiles(root, searchPatterns);
//
//	auto table = test_getType(pool, files);
//	cout << "========================================================================" << endl;
//	utils::Title("Requirement (6): Type Analysis.");
//	utils::Title("This program can detect the definition class, enum, struct, global function, alias and typedef.");
//	utils::Title("Moreover, it will eliminate the template definition and get the real name of the definition.");
//	cout << endl;
//	std::cout << std::setw(10) << "TYPE"
//		<< std::setw(20) << "DEFINITION"
//		<< std::setw(12) << "NAMESPACE"
//		<< "   " << "FILE PATH" << endl;
//	for (size_t i = 0; i < table.Size(); i++) {
//		std::cout << std::setw(10) << table[i].type
//			<< std::setw(20) << table[i].name
//			<< std::setw(12) << table[i].nameSpace 
//			<< "   " << table[i].filePath << endl << endl;
//		//cout << table[i].type << " " << table[i].name << " " << table[i].nameSpace << " " << table[i].filePath << endl;
//	}
//
//	auto dependencyTable = test_getDependency(pool, files, &table);
//	cout << "========================================================================" << endl;
//	utils::Title("Requirement (7, 8, 9, 10): Type Analysis Result.");
//	utils::Title("The result will only show different kinds of dependency(from file, depends file, dependency type and usage)");
//	utils::Title("You can see all types of dependency in the following result. ");
//	utils::Title("Including global function call, static function call, global lambda call, type(class, struct, enum) reference,  alias reference and inheritance.");
//	utils::Title("Moreover, you can look at the test files, the program can handle template, namespace and some complicated cases.");
//	cout << endl;
//	for (size_t i = 0; i < dependencyTable.Size(); i++) {
//		auto dep = dependencyTable[i];
//		std::cout << "Dependency Type: " << dep.type << endl;
//		std::cout << "From: " << dep.fromFile << endl;
//		std::cout << "Depends On: " << dep.toFile << endl;
//		//std::cout << "Usage: " << dep.usage << endl;
//		std::cout << "Type Definition: " << dep.definition.type << "  " + dep.definition.name << endl;
//		cout << "------------------------------------------------------------------------" << endl;
//		//cout << table[i].type << " " << table[i].name << " " << table[i].nameSpace << " " << table[i].filePath << endl;
//	}
//
//	std::cout << std::endl;
//	std::cout << "Press enter to exit..." << endl;
//	std::getline(std::cin, userInput);
//}