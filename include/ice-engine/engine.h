#include "common.h"
#include "network.h"
#include "graphics.h"
#include "system.h"
#include "event.h"
#include "threadpool.h"
#include <type_traits>
#ifndef ENGINE_H
#define ENGINE_H

namespace ice
{
    class engine
    {
    protected:
        ice::threadpool _threadpool;
    public:
        engine(size_t threads) : _threadpool(threads)
        {

        }

        template<class F, class... Args>
        std::future<typename std::result_of<F(Args...)>::type> call_async(
            F&& f, 
            Args&&...args) noexcept
        {
            return _threadpool.call_async(
                std::forward<F>(f), 
                std::forward<Args>(args)...);
        }

        template<class F, class... Args>
        std::result_of<F(Args...)> call(
            F&& f, 
            Args&&...args) noexcept
        {
            return _threadpool.call_async(
                std::forward<F>(f), 
                std::forward<Args>(args)...).get();
        }

        template<class F, class... Args>
        std::future<void> execute_async(
            F&& f, 
            Args&&...args) noexcept
        {
            _threadpool.execute_async(
                std::forward<F>(f), 
                std::forward<Args>(args)...);
        }

        template<class F, class... Args>
        void execute(
            F&& f, 
            Args&&...args) noexcept
        {
            _threadpool.execute_async(
                std::forward<F>(f), 
                std::forward<Args>(args)...).get();
        }
    };
    
#define TYPEDEF_EVENT_CALLBACK(EVENT_TYPE)                \
    typedef std::function<void(engine&,                   \
        const EVENT_TYPE ## &)> EVENT_TYPE ## _callback_t 

    TYPEDEF_EVENT_CALLBACK(key_event);
    TYPEDEF_EVENT_CALLBACK(mouse_event);
    TYPEDEF_EVENT_CALLBACK(quit_event);
    TYPEDEF_EVENT_CALLBACK(window_event);

}

#endif