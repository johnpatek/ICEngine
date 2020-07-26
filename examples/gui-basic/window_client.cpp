#include <ice-engine/graphics.h>
#include <thread>

int main(const int argc, const char ** argv)
{
    std::shared_ptr<ice::window> window;

    ice::init_graphics(ice::init_flags::ICE_INIT_VIDEO);

    window = std::make_shared<ice::window>(
        "Window Example",
        50,
        50,
        300,
        300,
        ice::window_flags::ICE_WINDOW_RESIZABLE);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    window->destroy();

    ice::quit_graphics();

    return 0;
}