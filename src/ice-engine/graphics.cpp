#include "ice-engine/graphics.h"
#include <SDL2/SDL.h>

#define HAS_FLAG(VAL, FLAG) ((VAL & FLAG) > 0)

static void tranlsate_init_flags(uint32_t * const sdl_flags, uint8_t flags)
{
    uint32_t result = 0;

    if(HAS_FLAG(flags,ice::init_flags::ICE_INIT_TIMER))
    {
        result |= SDL_INIT_TIMER;
    }

    if(HAS_FLAG(flags,ice::init_flags::ICE_INIT_AUDIO))
    {
        result |= SDL_INIT_AUDIO;
    }

    if(HAS_FLAG(flags,ice::init_flags::ICE_INIT_VIDEO))
    {
        result |= SDL_INIT_VIDEO;
    }

    if(HAS_FLAG(flags,ice::init_flags::ICE_INIT_JOYSTICK))
    {
        result |= SDL_INIT_JOYSTICK;
    }

    if(HAS_FLAG(flags,ice::init_flags::ICE_INIT_HAPTIC))
    {
        result |= SDL_INIT_HAPTIC;
    }

    if(HAS_FLAG(flags,ice::init_flags::ICE_INIT_GAMECONTROLLER))
    {
        result |= SDL_INIT_GAMECONTROLLER;
    }

    if(HAS_FLAG(flags,ice::init_flags::ICE_INIT_EVENTS))
    {
        result |= SDL_INIT_EVENTS;
    }

    if(HAS_FLAG(flags,ice::init_flags::ICE_INIT_EVERYTHING))
    {
        result |= SDL_INIT_EVERYTHING;
    }

    *sdl_flags = result;
}

static void tranlsate_window_flags(uint32_t * const sdl_flags, uint16_t flags)
{
    uint32_t result = 0;

    if(HAS_FLAG(flags,ice::window_flags::ICE_WINDOW_FULLSCREEN))
    {
        result |= SDL_WINDOW_FULLSCREEN;
    }

    if(HAS_FLAG(flags,ice::window_flags::ICE_WINDOW_FULLSCREEN_DESKTOP))
    {
        result |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    if(HAS_FLAG(flags,ice::window_flags::ICE_WINDOW_OPENGL))
    {
        result |= SDL_WINDOW_OPENGL;
    }

    if(HAS_FLAG(flags,ice::window_flags::ICE_WINDOW_VULKAN))
    {
        result |= SDL_WINDOW_VULKAN;
    }

    if(HAS_FLAG(flags,ice::window_flags::ICE_WINDOW_HIDDEN))
    {
        result |= SDL_WINDOW_HIDDEN;
    }

    if(HAS_FLAG(flags,ice::window_flags::ICE_WINDOW_BORDERLESS))
    {
        result |= SDL_WINDOW_BORDERLESS;
    }

    if(HAS_FLAG(flags,ice::window_flags::ICE_WINDOW_RESIZABLE))
    {
        result |= SDL_WINDOW_RESIZABLE;
    }

    if(HAS_FLAG(flags,ice::window_flags::ICE_WINDOW_MINIMIZED))
    {
        result |= SDL_WINDOW_MINIMIZED;
    }

    if(HAS_FLAG(flags,ice::window_flags::ICE_WINDOW_MAXIMIZED))
    {
        result |= SDL_WINDOW_MAXIMIZED;
    }

    if(HAS_FLAG(flags,ice::window_flags::ICE_WINDOW_INPUT_GRABBED))
    {
        result |= SDL_WINDOW_INPUT_GRABBED;
    }

    if(HAS_FLAG(flags,ice::window_flags::ICE_WINDOW_ALLOW_HIGHDPI))
    {
        result |= SDL_WINDOW_ALLOW_HIGHDPI;
    }

    *sdl_flags = result;
}

static void * init_window(
    const char * const title, 
    uint32_t x, 
    uint32_t y,
    uint32_t w,
    uint32_t h,
    uint32_t flags)
{
    if(x == ice::position_flags::ICE_POSITION_CENTERED)
    {
        x = SDL_WINDOWPOS_CENTERED;
    }

    if(y == ice::position_flags::ICE_POSITION_CENTERED)
    {
        y = SDL_WINDOWPOS_CENTERED;
    }

    if(x == ice::position_flags::ICE_POSITION_UNDEFINED)
    {
        x = SDL_WINDOWPOS_UNDEFINED;
    }

    if(y == ice::position_flags::ICE_POSITION_UNDEFINED)
    {
        y = SDL_WINDOWPOS_UNDEFINED;
    }

    return static_cast<void*>(SDL_CreateWindow(title,x,y,w,h,flags));
}

static void window_deleter(void * data)
{
    SDL_DestroyWindow(static_cast<SDL_Window*>(data));
}

void ice::init_graphics(const uint8_t flags)
{
    uint32_t sdl_flags;
    tranlsate_init_flags(&sdl_flags,flags);
    SDL_Init(sdl_flags);
}

void ice::quit_graphics()
{
    SDL_Quit();
}

ice::window::window()
{
    _window_data = nullptr;
}

ice::window::window(
    const std::string& title,
    uint32_t x, 
    uint32_t y, 
    uint32_t width, 
    uint32_t height, 
    uint16_t flags)
{
    create(title,x,y,width,height,flags);
}


ice::window::~window()
{
    if(_window_data.get() != nullptr)
    {
        destroy();
    }
}


void ice::window::create(
    const std::string& title,
    uint32_t x, 
    uint32_t y, 
    uint32_t width, 
    uint32_t height, 
    uint16_t flags)
{
    uint32_t sdl_flags;
    void * window_data;

    tranlsate_window_flags(&sdl_flags,flags);

    window_data = init_window(
        title.c_str(),x,y,width,height,sdl_flags);

    if(window_data == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }

    _window_data = std::shared_ptr<void>(window_data,window_deleter);
}

void ice::window::destroy()
{
    _window_data = nullptr;
}

