#include "ice-engine/common.h"
#include "ice-engine/engine.h"
#include "ice-engine/graphics.h"
#include "ice-engine/memory.h"
#include "ice-engine/network.h"
#include "ice-engine/threadpool.h"
#include "ice-engine/system.h"
#include <unordered_map>
#include <tuple>

/*
namespace ice
{
    class engine
    {
    private:
        ice::threadpool _threadpool;
        bool _has_error;
        std::string _error_buffer;
    public:
        template<class F, class... Args>
        std::future<typename std::result_of<F(Args...)>::type> call_async(F&& f, Args&&...args) noexcept
        {
            std::future<typename std::result_of<F(Args...)>::type> result;
            try
            {
                result = _threadpool.call_async(f,args...);
            }
            catch(const std::exception& e)
            {
                _has_error = true;
                _error_buffer = e;
            }
            return result;
        }

        template<class F, class... Args>
        std::result_of<F(Args...)> call(F&& f, Args&&...args) noexcept
        {
            std::result_of<F(Args...)> result;
            try
            {
                result = _threadpool.call(f,args...);
            }
            catch(const std::exception& e)
            {
                _has_error = true;
                _error_buffer = e;
            }
            return result;
        }

        template<class F, class... Args>
        std::future<typename std::result_of<F(Args...)>::type> delay_call_async(F&& f, Args&&...args) noexcept
        {
            std::future<typename std::result_of<F(Args...)>::type> result;
            try
            {
                result = _threadpool.call_async(f,args...);
            }
            catch(const std::exception& e)
            {
                _has_error = true;
                _error_buffer = e;
            }
            return result;
        }

        template<class F, class... Args>
        std::result_of<F(Args...)> delay_call(F&& f, Args&&...args) noexcept
        {
            std::result_of<F(Args...)> result;
            try
            {
                result = _threadpool.call(f,args...);
            }
            catch(const std::exception& e)
            {
                _has_error = true;
                _error_buffer = e;
            }
            return result;
        }

        bool has_error() const;

        const std::string& error() const;
    };
}*/