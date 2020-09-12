#include "netpong.h"

int main(const int argc, const char ** argv)
{
    std::shared_ptr<ice::window> window;

    ice::init_graphics(ice::init_flags::ICE_INIT_VIDEO);

    window = std::make_shared<ice::window>(
        "Window Example",
        ice::position_flags::ICE_POSITION_UNDEFINED,
        ice::position_flags::ICE_POSITION_UNDEFINED,
        1024,
        768,
        ice::window_flags::ICE_WINDOW_RESIZABLE);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    window->destroy();

    ice::quit_graphics();

    return 0;
}
