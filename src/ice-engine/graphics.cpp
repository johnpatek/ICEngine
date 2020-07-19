#include "ice-engine/graphics.h"
#include <SDL2/SDL.h>

static void * init_window(
    const char * const title, 
    uint32_t x, 
    uint32_t y,
    uint32_t w,
    uint32_t h,
    uint8_t flags)
{
    // return static_cast<void*>(SDL_CreateWindow(title,x,y,w,h,flags));
    return NULL;
}

static void window_deleter(void * data)
{
    // SDL_DestroyWindow(static_cast<SDL_Window*>(data));
}

ice::window::window(
    const std::string& title,
    uint32_t x, 
    uint32_t y, 
    uint32_t width, 
    uint32_t height, 
    uint8_t flags)
{
    void * window_data;

    window_data = init_window(
        title.c_str(),x,y,width,height,flags);

    if(window_data == nullptr)
    {
        // throw std::runtime_error(SDL_GetError());
    }

    _window_data = std::shared_ptr<void>(window_data,window_deleter);
    
}
