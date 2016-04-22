#ifndef PARALLELEXECUTIVE_H
#define PARALLELEXECUTIVE_H
#pragma once

/*
* ParallelExecutive.h
* This file do some teses using the threadpool, type analysis and dependence analysis.
* It will accept some file search patterns and do parallel analysis with multiple threads.
*
* Author: Junkai Yang
*/
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <future>
#include "../Tokenizer/Utils.h"
#include "../FileSystem_Windows/FileSystem.h"
#include "../ThreadHelper/ThreadPool.h"
#include "../FileSystem_Windows/FileSystemSearchHelper.h"
#include "../DependencyAnalysis/TypeTable.h"
#include "../DependencyAnalysis/TypeAnalysis.h"
#include "../DependencyAnalysis/DependencyAnalysis.h"
#include "../DependencyAnalysis/DependencyDetector.h"
#include "../DependencyAnalysis/DependencyTable.h"


TypeTable test_getType(ThreadPool& pool, vector<string> files);

DependencyTable test_getDependency(ThreadPool& pool, vector<string> files, TypeTable* typeTable);
#endif
