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
#include <shared_mutex>

namespace ice
{

class threadpool
{
private:
    /* data */
    std::queue<std::function<void()>> work_queue;
    std::vector<std::thread> worker_threads;
    std::condition_variable_any thread_cv;
    mutable std::shared_timed_mutex cv_mutex;
    bool mStop;
public:
    threadpool(size_t num_threads = std::thread::hardware_concurrency());
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
          std::unique_lock<std::shared_timed_mutex> _lock(cv_mutex);
          work_queue.push(work);
       }

       thread_cv.notify_one();
       return result;
    }

   template<class F, class... Args>
   std::result_of<F(Args...)> call(F&& f, Args&&...args)
   {
      return call_async(f,args...).wait();
   }

    void run_worker_thread(void);

    ~threadpool();

   void shutdown();

   bool has_work() const;

   size_t work() const;

};



class priority_threadpool
{
private:

   using work_t = std::pair<int,std::function<void()>>;

   class compare_work
   {
   public:
      bool operator() (work_t& a, work_t& b)
      {
         return a.first < b.first;
      }
   };

   std::vector<std::thread> _threads;
   
   std::priority_queue<
      work_t,
      std::vector<work_t>,
      compare_work> _queue;
   
   mutable std::shared_timed_mutex _mutex;
   
   std::condition_variable_any _cv;
   
   bool _stop;

public:

   priority_threadpool(size_t pool_size = std::thread::hardware_concurrency());

   ~priority_threadpool();

   template<class F, class... Args>
   std::future<typename std::result_of<F(Args...)>::type> call_async(
      int priority,F&& f, Args&&...args)
   {
      std::unique_lock<std::shared_timed_mutex> lock(_mutex, std::defer_lock);
      using return_type = typename std::result_of<F(Args...)>::type;
      work_t work;

      work.first = priority;

      auto task = std::make_shared<std::packaged_task <return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
      
      std::future<return_type> result = task->get_future();

      std::function<void()> function([task]()
      {
         (*task)();
      });

      work.second = function;

      lock.lock();
      _queue.push(work);
      lock.unlock();
      
      _cv.notify_one();

      return result;
   }   

   template<class F, class... Args>
   std::result_of<F(Args...)> call(
      int priority,F&& f, Args&&...args)
   {
      return call_async(priority,f,args...).wait();
   }

   void shutdown();

   bool has_work() const;

   size_t work() const;
};

}
#endif