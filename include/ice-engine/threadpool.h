#ifndef THREADPOOL_H
#define THREADPOOL_H
//#include "common.h"
#include <functional>
#include <iostream>
#include <future>
#include <thread>
#include <queue>
#include <vector>
#include <condition_variable>

class threadpool
{
private:
    /* data */
    std::queue<std::function<void()>> work_queue;
    std::vector<std::thread> worker_threads;
    std::condition_variable thread_cv;
    std::mutex cv_mutex;
    bool mStop;
public:
    threadpool(uint8_t num_threads);
    template<class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> call_async(F&& f, Args&&...args)
    {
       using return_type = typename std::result_of<F(Args...)>::type;

       auto task = std::make_shared<std::packaged_task <return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
       std::future<return_type> result = task->get_future();

       std::function<void()> work([task]()
       {
          (*task)();
       });

       // context for releasing lock after shared value is modified
       {
          std::unique_lock<std::mutex> _lock(cv_mutex);
          work_queue.push(work);
       }

       thread_cv.notify_all();
       return result;
    }

    void run_worker_thread(void);
    ~threadpool();
};
#endif
