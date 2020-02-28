#include <uWS/uWS.h>
#include <uWS/Hub.h>
#include <uWS/Libuv.h>
#include <thread>
#include <algorithm>
#include <iostream>
#include <unistd.h>

void close_hub_event_loop(uv_async_t * handle)
{
    auto uv_walk_callback = [](uv_handle_t * handle, void* pass)
    {
        if(!uv_is_closing(handle))
        {
            uv_close(handle, nullptr);
        }
    };

    auto loop = handle->loop;
    auto hub = (uWS::Hub *)handle->data;
    uv_stop(loop);
    uv_walk(loop,uv_walk_callback,nullptr);
    uv_run(loop,UV_RUN_DEFAULT);
    uv_loop_close(loop);
}

const char * const response = "This is a response";

int main(const int argc, const char ** argv) 
{
    const size_t hardware_threads = 4 * std::thread::hardware_concurrency();
    std::vector<std::unique_ptr<std::thread>> threads;
    std::vector<std::unique_ptr<uv_async_t>> async_handles;
    for(int i = 0; i < hardware_threads; i++)
    {
        std::unique_ptr<uWS::Hub> hub(new uWS::Hub);
        std::unique_ptr<uv_async_t> async_handle(new uv_async_t);
        
        async_handle->data = hub.get();

        uv_async_init(
            (uv_loop_t*)hub->getLoop(),
            async_handle.get(),
            close_hub_event_loop);
        
        hub->onHttpData([](
            uWS::HttpResponse * res, 
            char * buf, 
            size_t size, 
            size_t remain){
                std::cerr << "onHttpData" << std::endl;
            });
        
        hub->onHttpRequest([](
            uWS::HttpResponse * res, 
            uWS::HttpRequest req, 
            char * buf, 
            size_t size, 
            size_t remain){
                std::cerr << std::string(buf,size) << std::endl;
                res->end(response,strlen(response));
            });
        
        hub->listen(3000,nullptr,uS::ListenOptions::REUSE_PORT);

        hub->getDefaultGroup<uWS::SERVER>().addAsync();

        async_handles.push_back(
            std::move(async_handle));

        threads.push_back(std::unique_ptr<std::thread>(
            new std::thread([](std::unique_ptr<uWS::Hub> && h)
            {   
                h->run();
                h.reset();
            }, std::move(hub))));
        
    }
    pause();
    return 0;
}
