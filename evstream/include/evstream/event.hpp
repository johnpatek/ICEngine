#ifndef EVSTREAM_EVENT_HPP
#define EVSTREAM_EVENT_HPP

#include "common.hpp"
#include <SDL3/SDL.h>

namespace evstream
{
    struct event
    {
        enum class type
        {
            keyboard,
            mouse_button,
            mouse_motion,
            mouse_wheel,
        };
        std::variant<> data;
    };

    void d()
    {
        SDL_KeyboardEvent k;
        k.scancode = SDL_SCANCODE_A; // Example usage of SDL_Scancode
    }
}

#endif