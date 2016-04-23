/*
*	ThreadPool.h
*	This is the a simple thread pool implementation.
*	It holds specified thread and let them running all the time.
*	Once new task arrives, one of the thread take it out, execute it and return the result
*	
*	Author: Junkai Yang
*	1. Add comments to make it easier to modify later
*	2. Reconstruct the lambda usage in code, make it easier to read and understand
*/
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {
public:
	ThreadPool(size_t);
	
	template<class Fn, class... Args>
	auto enqueue(Fn&& f, Args&&... args)
		->std::future<typename std::result_of<Fn(Args...)>::type>;

	~ThreadPool();
private:
	// need to keep track of threads so we can join them
	std::vector< std::thread > workers;

	// the task queue
	std::queue< std::function<void()> > tasks;

	// synchronization
	std::mutex queue_mutex;
	std::condition_variable condition;
	bool stop;
};


// add new work item to the pool
// Dont know why but the compile fails if I move this function to threadpool.cpp
template<typename Fn, typename... Args>
auto ThreadPool::enqueue(Fn&& fn, Args&&... args)
-> std::future<typename std::result_of<Fn(Args...)>::type>
{
	using return_type = typename std::result_of<Fn(Args...)>::type;
	auto task = std::make_shared< std::packaged_task<return_type()> >(
		std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...)
		);

	std::future<return_type> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(queue_mutex);

		// don't allow enqueueing after stopping the pool
		if (stop)
			throw std::runtime_error("enqueue on stopped ThreadPool");

		tasks.emplace([task]() { (*task)(); });
	}
	condition.notify_one();
	return res;
}


#endif