#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <memory>
#include <stdexcept>
#include <string>
#include "common.h"
#include "event.h"
#ifdef _WIN32
#undef WIN32_HIGH_RES_TIMING
#include <windows.h>
#include <commctrl.h>
#include <timeapi.h>
#include <mmsystem.h>
#include <SDL2/SDL_config_windows.h>
#pragma comment (lib, "Setupapi.lib")
#pragma comment (lib, "version.lib")
#pragma comment(lib, "secur32.lib")
#pragma comment(lib, "dmoguids.lib")
#pragma comment(lib, "wmcodecdspuuid.lib")
#pragma comment(lib, "msdmo.lib")
#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "ws2_32.lib ")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "hid.lib")
#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "WinMM.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "tapi32.lib")
#endif

namespace ice
{
    class renderer;
    class surface;
    class texture;
    class window;
    
    enum init_flags
    {
        ICE_INIT_TIMER = 1,
        ICE_INIT_AUDIO = 1 << 1,
        ICE_INIT_VIDEO = 1 << 2 ,
        ICE_INIT_JOYSTICK = 1 << 3,
        ICE_INIT_HAPTIC = 1 << 4,
        ICE_INIT_GAMECONTROLLER = 1 << 5,
        ICE_INIT_EVENTS = 1 << 6,
        ICE_INIT_EVERYTHING = 127
    };

    void init_system(int flags);

    void quit_system();

    bool poll_system_events(
        std::pair<bool,key_event>& key_event_listener,
        std::pair<bool,mouse_event>& mouse_event_listener,
        std::pair<bool,quit_event>& quit_event_listener,
        std::pair<bool,window_event>& window_event_listener);

    enum window_flags
    {
        ICE_WINDOW_FULLSCREEN = 1,
        ICE_WINDOW_FULLSCREEN_DESKTOP = 1 << 1,
        ICE_WINDOW_OPENGL = 1 << 2,
        ICE_WINDOW_VULKAN = 1 << 3,
        ICE_WINDOW_HIDDEN = 1 << 4,
        ICE_WINDOW_BORDERLESS = 1 << 5,
        ICE_WINDOW_RESIZABLE = 1 << 6,
        ICE_WINDOW_MINIMIZED = 1 << 7,
        ICE_WINDOW_MAXIMIZED = 1 << 8,
        ICE_WINDOW_INPUT_GRABBED = 1 << 9,
        ICE_WINDOW_ALLOW_HIGHDPI = 1 << 10
    };

    enum position_flags
    {
        ICE_POSITION_UNDEFINED,
        ICE_POSITION_CENTERED
    };

    class window
    {
    private:
        std::shared_ptr<void> _window_data;
    public:
        window();

        window(
            const std::string& title,
            size_t x, 
            size_t y, 
            size_t width, 
            size_t height, 
            int flags);
        
        ~window();

        void create(
            const std::string& title,
            size_t x, 
            size_t y, 
            size_t width, 
            size_t height, 
            int flags);
        
        void destroy();

        void * get_data();

        void get_size(
            size_t * const width, 
            size_t * const height)/* const? */;
        
        void set_size(
            size_t width, 
            size_t height);

    };


    struct rectangle
    {
        int x_position;
        int y_position;
        int width;
        int height;
    };

    enum renderer_flags
    {
        ICE_RENDERER_SOFTWARE = 1,
        ICE_RENDERER_ACCELERATED = 1 << 1,
        ICE_RENDERER_PRESENTVSYNC = 1 << 2,
        ICE_RENDERER_TARGETTEXXTURE = 1 << 3
    };

    class renderer
    {
    private:
        std::shared_ptr<void> _renderer_data;
    public:
        renderer();

        renderer(window& wind, int index, int flags);

        ~renderer();

        void create(window& wind, int index, int flags);

        void destroy();

        void * get_data();

        void present();

        void clear();

        void draw_rectangle(const rectangle& rect);

        void fill_rectangle(const rectangle& rect);

        void set_target(texture& txtr);

        void set_target_default();

        void get_draw_color(
            uint8_t& red, 
            uint8_t& green, 
            uint8_t& blue, 
            uint8_t& alpha);

        void set_draw_color(
            uint8_t red, 
            uint8_t green, 
            uint8_t blue, 
            uint8_t alpha);

        void copy(texture& src_txtr);

        void partial_copy_source(
            texture& src_txtr, 
            const rectangle& src_rect);

        void partial_copy_destination(
            texture& src_txtr, 
            const rectangle& dst_rect);

        void partial_copy(
            texture& src_txtr, 
            const rectangle& src_rect, 
            const rectangle& dst_rect);
    };

    class surface
    {
    private:
        std::shared_ptr<void> _surface_data;
    public:

    };

    class texture
    {
    private:
        std::shared_ptr<void> _texture_data;
    public:
        texture();

        texture(
            renderer& rend, 
            int format, 
            int access, 
            int width, 
            int height);
        
        void create(
            renderer& rend, 
            int format, 
            int access, 
            int width, 
            int height);

        void destory();

        void * get_data();
    };

    
}

#endif