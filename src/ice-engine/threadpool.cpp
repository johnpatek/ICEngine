//#include "common.h"
#include "ice-engine/threadpool.h"
#include <functional>
#include <iostream>
#include <future>
#include <thread>
#include <queue>
#include <vector>
#include <condition_variable>

threadpool::threadpool(uint8_t num_threads)
{
    //
    worker_threads.reserve(num_threads);
    // initialize to be running a worker task
    // worker task should simply use cond variable to access whether or not there is work
    int i = 0;
    mStop = false;
    while(worker_threads.size() < worker_threads.capacity())
    {
        worker_threads.push_back(std::thread([this]
        {
           this->run_worker_thread();
	}));
	worker_threads[i].detach();
	++i;
    }
}


void threadpool::run_worker_thread(void)
{
    while(1)
    {
        std::function<void()> work_task;
        {
            std::unique_lock<std::shared_timed_mutex> _lock(cv_mutex);
	    std::cout<< "thread_id: "<< std::this_thread::get_id() << std::endl;
            thread_cv.wait(_lock, [&]{return (!work_queue.empty() || mStop); }); // or stopping
            if(mStop)
	    {
                std::cout<< "exiting thread_id: "<< std::this_thread::get_id() << std::endl;
                break;
	    } 
            std::cout<<"we're here"<<std::endl;
	    work_task = work_queue.front();
	    work_queue.pop();
	}
        work_task();
    }
}

threadpool::~threadpool()
{
    std::unique_lock<std::shared_timed_mutex> _lock(cv_mutex);
    mStop = true;
    thread_cv.notify_all();
}
