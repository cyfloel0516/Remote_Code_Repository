/*
* ThreadPool.cpp
* Include the test stub for thread pool
* 
* Author: Junkai Yang
*/
#include <iostream>
#include <vector>
#include <chrono>
#include <future>

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include "ThreadPool.h"


// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
	: stop(false)
{
	auto worker = [this]()
	{
		while (!stop)
		{
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(this->queue_mutex);
				this->condition.wait(lock,
					[this]() { return this->stop || !this->tasks.empty(); });

				if (this->stop && this->tasks.empty())
					return;

				task = std::move(this->tasks.front());
				this->tasks.pop();
			}

			task();
		}
	};

	for (size_t i = 0;i<threads;++i)
		workers.emplace_back(worker);
}

// the destructor joins all threads
ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}
	condition.notify_all();
	for (std::thread &worker : workers)
		worker.join();
}



#ifdef THREADPOOL_TEST
int main()
{
	ThreadPool pool(4);
	std::vector< std::future<int> > results;

	for (int i = 0; i < 8; ++i) {
		results.emplace_back(
			pool.enqueue([i](){
			std::cout << "hello " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "world " << i << std::endl;
			return i*i;
		})
			);
	}

	for (auto && result : results)
		std::cout << result.get() << ' ';
	std::cout << std::endl;

	return 0;
}
#endif