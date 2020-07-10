#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <memory>
#include <stdexcept>
#include <string>

namespace ice
{
    void init_graphics();

    void deinit_graphics();

    class window
    {
    private:
        std::shared_ptr<void> _window_data;
    public:
        window(
            const std::string& title,
            uint32_t x, 
            uint32_t y, 
            uint32_t width, 
            uint32_t height, 
            uint8_t flags);
        
        ~window() = default;
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