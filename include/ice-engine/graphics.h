#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <memory>
#include <stdexcept>
#include <string>
#include "common.h"
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

// As specified by Stack Overflow
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

    void init_graphics(const uint8_t flags);

    void quit_graphics();

    class window
    {
    private:
        std::shared_ptr<void> _window_data;
    public:
        window();

        window(
            const std::string& title,
            uint32_t x, 
            uint32_t y, 
            uint32_t width, 
            uint32_t height, 
            uint16_t flags);
        
        ~window();

        void create(
            const std::string& title,
            uint32_t x, 
            uint32_t y, 
            uint32_t width, 
            uint32_t height, 
            uint16_t flags);
        
        void destroy();

        void get_size(
            uint32_t * const width, 
            uint32_t * const height)/* const? */;
        
        void set_size(
            uint32_t width, 
            uint32_t height);
    };

    class surface
    {
    private:
        std::shared_ptr<void> _surface_data;
    public:

    };

    class renderer
    {
    private:
        std::shared_ptr<void> _renderer_data;
    public:

    };
}

#endif