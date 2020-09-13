#include "netpong.h"

int main(const int argc, const char ** argv)
{
    std::shared_ptr<ice::window> window;
    std::shared_ptr<ice::renderer> renderer;
    std::shared_ptr<ice::texture> texture;

    ice::init_system(ice::init_flags::ICE_INIT_VIDEO);

    window = std::make_shared<ice::window>(
        "Window Example",
        ice::position_flags::ICE_POSITION_UNDEFINED,
        ice::position_flags::ICE_POSITION_UNDEFINED,
        1024,
        768,
        ice::window_flags::ICE_WINDOW_RESIZABLE);

    renderer = std::make_shared<ice::renderer>(
        *window,
        -1,
        0);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    texture->destroy();
    renderer->destory();
    window->destroy();

    ice::quit_system();

    return 0;
}
