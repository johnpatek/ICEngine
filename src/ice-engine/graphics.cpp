#include "ice-engine/graphics.h"
#include <SDL2/SDL.h>

#define HAS_FLAG(VAL, FLAG) ((VAL & FLAG) > 0)

static void tranlsate_init_flags(int * const sdl_flags, int flags)
{
    int result = 0;

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

void init_system(int flags)
{

}

void quit_system()
{

}

static void tranlsate_window_flags(int * const sdl_flags, int flags)
{
    int result = 0;

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
    size_t x, 
    size_t y,
    size_t w,
    size_t h,
    int flags)
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

    return static_cast<void*>(SDL_CreateWindow(
        title,
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(w),
        static_cast<int>(h),
        static_cast<uint32_t>(flags)));
}

static void window_deleter(void * data)
{
    SDL_DestroyWindow(static_cast<SDL_Window*>(data));
}

void ice::init_system(int flags)
{
    int sdl_flags;
    tranlsate_init_flags(&sdl_flags,flags);
    if(SDL_Init(sdl_flags) < 0)
    {
        throw std::runtime_error("System failed to initialize");
    }
}

void ice::quit_system()
{
    SDL_Quit();
}

ice::window::window()
{
    _window_data = nullptr;
}

ice::window::window(
    const std::string& title,
    size_t x, 
    size_t y, 
    size_t width, 
    size_t height, 
    int flags)
{
    create(title,x,y,width,height,flags);
}


ice::window::~window()
{
    if(_window_data.unique() && _window_data.get() != nullptr)
    {
        destroy();
    }
}


void ice::window::create(
    const std::string& title,
    size_t x, 
    size_t y, 
    size_t width, 
    size_t height, 
    int flags)
{
    int sdl_flags;
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

static void translate_renderer_flags(int * const sdl_flags, int flags)
{
    int result;

    if(HAS_FLAG(flags,ice::renderer_flags::ICE_RENDERER_SOFTWARE))
    {
        result |= SDL_RENDERER_SOFTWARE;
    }

    if(HAS_FLAG(flags,ice::renderer_flags::ICE_RENDERER_ACCELERATED))
    {
        result |= SDL_RENDERER_ACCELERATED;
    }

    if(HAS_FLAG(flags,ice::renderer_flags::ICE_RENDERER_PRESENTVSYNC))
    {
        result |= SDL_RENDERER_PRESENTVSYNC;
    }

    if(HAS_FLAG(flags,ice::renderer_flags::ICE_RENDERER_TARGETTEXXTURE))
    {
        result |= SDL_WINDOW_VULKAN;
    }

    *sdl_flags = result; 
}

static void * init_renderer(SDL_Window * const window, int index, int flags)
{
    return SDL_CreateRenderer(window,index,flags);
}

static void renderer_deleter(void * renderer)
{
    SDL_DestroyRenderer(
        reinterpret_cast<SDL_Renderer*>(renderer));
}

ice::renderer::renderer()
{
    _renderer_data = nullptr;
}

ice::renderer::renderer(ice::window& wind, int index, int flags)
{
    create(wind,index,flags);
}

ice::renderer::~renderer()
{
    if(_renderer_data.unique() && _renderer_data != nullptr)
    {
        destroy();
    }
}

void ice::renderer::create(ice::window& wind, int index, int flags)
{
    int sdl_flags = 0;
    translate_renderer_flags(&sdl_flags,flags);
    _renderer_data = std::shared_ptr<void>(
        init_renderer(
            reinterpret_cast<SDL_Window*>(
                wind.get_data()),
            index,
            sdl_flags),
        renderer_deleter);
}

void ice::renderer::destroy()
{
    _renderer_data = nullptr;
}

void * ice::renderer::get_data()
{
    return _renderer_data.get();
}

void ice::renderer::present()
{
    SDL_RenderPresent(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()));
}

void ice::renderer::clear()
{
    SDL_RenderClear(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()));
}

void ice::renderer::draw_rectangle(
    const ice::rectangle& rect)
{
    SDL_RenderDrawRect(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()),
        reinterpret_cast<const SDL_Rect*>(
            &rect));
}

void ice::renderer::fill_rectangle(
    const ice::rectangle& rect)
{
    SDL_RenderFillRect(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()),
        reinterpret_cast<const SDL_Rect*>(
            &rect));
}

void ice::renderer::set_target(
    ice::texture& txtr)
{
    SDL_SetRenderTarget(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()),
        reinterpret_cast<SDL_Texture*>(
            &txtr));
}

void ice::renderer::set_target_default()
{
    SDL_SetRenderTarget(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()),
        nullptr);
}

void ice::renderer::get_draw_color(
    uint8_t& red, 
    uint8_t& green, 
    uint8_t& blue, 
    uint8_t& alpha)
{
    SDL_GetRenderDrawColor(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()),
        &red,
        &green,
        &blue,
        &alpha);
}

void ice::renderer::set_draw_color(
    uint8_t red, 
    uint8_t green, 
    uint8_t blue, 
    uint8_t alpha)
{
    SDL_SetRenderDrawColor(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()),
        red,
        green,
        blue,
        alpha);
}

void ice::renderer::copy(
    ice::texture& src_txtr)
{
    SDL_RenderCopy(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()),
        reinterpret_cast<SDL_Texture*>(
            &src_txtr),
        nullptr,
        nullptr);
}

void ice::renderer::partial_copy_source(
    ice::texture& src_txtr, 
    const ice::rectangle& src_rect)
{
    SDL_RenderCopy(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()),
        reinterpret_cast<SDL_Texture*>(
            &src_txtr),
        reinterpret_cast<const SDL_Rect*>(
            &src_rect),
        nullptr);
}

void ice::renderer::partial_copy_destination(
    ice::texture& src_txtr, 
    const ice::rectangle& dst_rect)
{
    SDL_RenderCopy(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()),
        reinterpret_cast<SDL_Texture*>(
            &src_txtr),
        nullptr,
        reinterpret_cast<const SDL_Rect*>(
            &dst_rect));
}

void ice::renderer::partial_copy(
    ice::texture& src_txtr, 
    const ice::rectangle& src_rect, 
    const ice::rectangle& dst_rect)
{
    SDL_RenderCopy(
        reinterpret_cast<SDL_Renderer*>(
            _renderer_data.get()),
        reinterpret_cast<SDL_Texture*>(
            &src_txtr),
        reinterpret_cast<const SDL_Rect*>(
            &src_rect),
        reinterpret_cast<const SDL_Rect*>(
            &dst_rect) );
}

static void create_key_event(ice::key_event& event, const SDL_KeyboardEvent& sdl_event);

bool poll_system_events(
    std::pair<bool,ice::key_event>& key_event_listener,
    std::pair<bool,ice::mouse_event>& mouse_event_listener,
    std::pair<bool,ice::quit_event>& quit_event_listener,
    std::pair<bool,ice::window_event>& window_event_listener)
{
    SDL_Event sdl_event;
    key_event_listener.first = false;
    mouse_event_listener.first = false;
    quit_event_listener.first = false;
    window_event_listener.first = false;
    if(SDL_PollEvent(&sdl_event) == 1)
    {
        switch(sdl_event.type)
        {
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                key_event_listener.first = true;
                create_key_event(key_event_listener.second,sdl_event.key);
                break;
            case SDL_QUIT:
                quit_event_listener.first = true;
                break;
        }
    }

    return true;
}

static void create_key_event(ice::key_event& event, const SDL_KeyboardEvent& sdl_event)
{
    if(sdl_event.type == SDL_KEYUP)
    {
        event.set_event_type(
            ice::key_event_types::ICE_KEY_RELEASED);
    }
    else if(sdl_event.type == SDL_KEYDOWN)
    {
        event.set_event_type(
            ice::key_event_types::ICE_KEY_PRESSED);
    }
    else
    {
        // Make sure this is unreachable
    }

    event.set_repeat(sdl_event.repeat != 0);

    switch(sdl_event.keysym.sym)
    {
#define KEY_CASE(CODE)                                               \
    case SDLK_ ## CODE ## :                                          \
        event.set_key_code(                                          \
            ice::key_codes::ICE_KEY_CODE_ ## CODE ## );              \
        break                                                        
        KEY_CASE(BACKSPACE);
        KEY_CASE(TAB);
        KEY_CASE(CLEAR);
        KEY_CASE(RETURN);
        KEY_CASE(PAUSE);
        KEY_CASE(SPACE);
        KEY_CASE(EXCLAIM);
        KEY_CASE(QUOTEDBL);
        KEY_CASE(HASH);
        KEY_CASE(DOLLAR);
        KEY_CASE(AMPERSAND);
        KEY_CASE(QUOTE);
        KEY_CASE(LEFTPAREN);
        KEY_CASE(RIGHTPAREN);
        KEY_CASE(ASTERISK);
        KEY_CASE(PLUS);
        KEY_CASE(COMMA);
        KEY_CASE(MINUS);
        KEY_CASE(PERIOD);
        KEY_CASE(SLASH);
        KEY_CASE(0);
        KEY_CASE(1);
        KEY_CASE(2);
        KEY_CASE(3);
        KEY_CASE(4);
        KEY_CASE(5);
        KEY_CASE(6);
        KEY_CASE(7);
        KEY_CASE(8);
        KEY_CASE(9);
        KEY_CASE(COLON);
        KEY_CASE(SEMICOLON);
        KEY_CASE(LESS);
        KEY_CASE(EQUALS);
        KEY_CASE(GREATER);
        KEY_CASE(QUESTION);
        KEY_CASE(AT);
        KEY_CASE(LEFTBRACKET);
        KEY_CASE(BACKSLASH);
        KEY_CASE(RIGHTBRACKET);
        KEY_CASE(CARET);
        KEY_CASE(UNDERSCORE);
        KEY_CASE(BACKQUOTE);
        KEY_CASE(a);
        KEY_CASE(b);
        KEY_CASE(c);
        KEY_CASE(d);
        KEY_CASE(e);
        KEY_CASE(f);
        KEY_CASE(g);
        KEY_CASE(h);
        KEY_CASE(i);
        KEY_CASE(j);
        KEY_CASE(k);
        KEY_CASE(l);
        KEY_CASE(m);
        KEY_CASE(n);
        KEY_CASE(o);
        KEY_CASE(p);
        KEY_CASE(q);
        KEY_CASE(r);
        KEY_CASE(s);
        KEY_CASE(t);
        KEY_CASE(u);
        KEY_CASE(v);
        KEY_CASE(w);
        KEY_CASE(x);
        KEY_CASE(y);
        KEY_CASE(z);
        KEY_CASE(DELETE);
        // KEY_CASE(KP0);
        // KEY_CASE(KP1);
        // KEY_CASE(KP2);
        // KEY_CASE(KP3);
        // KEY_CASE(KP4);
        // KEY_CASE(KP5);
        // KEY_CASE(KP6);
        // KEY_CASE(KP7);
        // KEY_CASE(KP8);
        // KEY_CASE(KP9);
        KEY_CASE(KP_PERIOD);
        KEY_CASE(KP_DIVIDE);
        KEY_CASE(KP_MULTIPLY);
        KEY_CASE(KP_MINUS);
        KEY_CASE(KP_PLUS);
        KEY_CASE(KP_ENTER);
        KEY_CASE(KP_EQUALS);
        KEY_CASE(UP);
        KEY_CASE(DOWN);
        KEY_CASE(RIGHT);
        KEY_CASE(LEFT);
        KEY_CASE(INSERT);
        KEY_CASE(HOME);
        KEY_CASE(END);
        KEY_CASE(PAGEUP);
        KEY_CASE(PAGEDOWN);
        KEY_CASE(F1);
        KEY_CASE(F2);
        KEY_CASE(F3);
        KEY_CASE(F4);
        KEY_CASE(F5);
        KEY_CASE(F6);
        KEY_CASE(F7);
        KEY_CASE(F8);
        KEY_CASE(F9);
        KEY_CASE(F10);
        KEY_CASE(F11);
        KEY_CASE(F12);
        KEY_CASE(F13);
        KEY_CASE(F14);
        KEY_CASE(F15);
        // KEY_CASE(NUMLOCK);
        KEY_CASE(CAPSLOCK);
        // KEY_CASE(SCROLLOCK);
        KEY_CASE(RSHIFT);
        KEY_CASE(LSHIFT);
        KEY_CASE(RCTRL);
        KEY_CASE(LCTRL);
        KEY_CASE(RALT);
        KEY_CASE(LALT);
        // KEY_CASE(RMETA);
        // KEY_CASE(LMETA);
        // KEY_CASE(LSUPER);
        // KEY_CASE(RSUPER);
        KEY_CASE(MODE);
        KEY_CASE(HELP);
        // KEY_CASE(PRINT);
        KEY_CASE(SYSREQ);
        // KEY_CASE(BREAK);
        KEY_CASE(MENU);
        KEY_CASE(POWER);
        // KEY_CASE(EURO);
        default:
            event.set_key_code(0);
            break;
    }    


}
