//#include "common.h"
#include "ice-engine/threadpool.h"
#include <functional>
#include <iostream>
#include <future>
#include <thread>
#include <queue>
#include <vector>
#include <condition_variable>

ice::threadpool::threadpool(size_t num_threads)
{
    //
    worker_threads.reserve(num_threads);
    // initialize to be running a worker task
    // worker task should simply use cond variable to access whether or not there is work
    mStop = false;
    while(worker_threads.size() < worker_threads.capacity())
    {
        worker_threads.push_back(std::thread([&]
        {
            std::unique_lock<std::shared_timed_mutex> lock(
                cv_mutex, 
                std::defer_lock);
            while(!mStop)
            {
                std::function<void()> job;
            
                lock.lock();
                
                thread_cv.wait(lock,[&]
                {
                    return mStop || !work_queue.empty();
                });
                
                if(!work_queue.empty())
                {
                    job = std::move(work_queue.front());
                    work_queue.pop();
                }

                lock.unlock();

                if(job)
                {
                    job();
                }
            }
	    }));
    }
}

ice::threadpool::~threadpool()
{
    if(!mStop)
    {
        shutdown();
    }
}

void ice::threadpool::shutdown()
{
    std::unique_lock<std::shared_timed_mutex> lock(cv_mutex, std::defer_lock);
    lock.lock();
    mStop = true;
    lock.unlock();
    thread_cv.notify_all();
    for(std::thread & thread : worker_threads)
    {
        thread.join();
    }
}

bool ice::threadpool::has_work() const
{
    std::shared_lock<std::shared_timed_mutex> lock(cv_mutex);
    return !work_queue.empty();
}

size_t ice::threadpool::work() const
{
    std::shared_lock<std::shared_timed_mutex> lock(cv_mutex);
    return work_queue.size();
}

ice::priority_threadpool::priority_threadpool(size_t pool_size)
{
    _threads.reserve(pool_size);
    
    _stop = false;
    
    while(_threads.size() < pool_size)
    {
        _threads.push_back(std::thread([&]
        {
            std::unique_lock<std::shared_timed_mutex> lock(
                _mutex, std::defer_lock);
            std::function<void()> job;
            while(!_stop)
            {
                lock.lock();
                _cv.wait(lock,[&]
                {
                    return _stop || !_queue.empty();
                });
                
                if(!_queue.empty())
                {
                    job = std::move(
                        _queue.top().second);
                    _queue.pop();
                    lock.unlock();
                }

                if(job)
                {
                    job();
                }
            }
        }));        
    }
}

ice::priority_threadpool::~priority_threadpool()
{
    if(!_stop)
    {
        shutdown();
    }
}

void ice::priority_threadpool::shutdown()
{
    std::unique_lock<std::shared_timed_mutex> lock(_mutex, std::defer_lock);
    lock.lock();
    _stop = true;
    lock.unlock();
    _cv.notify_all();
    for(std::thread & thread : _threads)
    {
        thread.join();
    }
}

bool ice::priority_threadpool::has_work() const
{
    std::shared_lock<std::shared_timed_mutex> lock(_mutex);
    return !_queue.empty();
}

size_t ice::priority_threadpool::work() const
{
    std::shared_lock<std::shared_timed_mutex> lock(_mutex);
    return _queue.size();
}