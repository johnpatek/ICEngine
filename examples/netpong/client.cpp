#include "netpong.h"




int main(const int argc, const char ** argv)
{
    ice::client client(8);
    std::shared_ptr<ice::window> window;
    std::shared_ptr<ice::renderer> renderer;
    std::shared_ptr<ice::texture> texture;
    ice::rectangle rectangle;

    client.set_init_function([&](ice::engine& engine)
    {
        window = std::make_shared<ice::window>(
            "netpong",
            ice::position_flags::ICE_POSITION_CENTERED,
            ice::position_flags::ICE_POSITION_CENTERED,
            640,
            480);
        
        renderer = std::make_shared<ice::renderer>(*window,-1,0);

        texture = std::make_shared<ice::texture>();
    });

    client.set_loop_function([&](ice::engine& engine)
    {
        renderer->set_target(*texture);
        renderer->set_draw_color(0x00,0x00,0x00,0x00);
        renderer->clear();
        renderer->draw_rectangle(rectangle);
        renderer->set_draw_color(0xFF,0x00,0x00,0x00);
        renderer->fill_rectangle(rectangle);
        renderer->set_target_default();
        renderer->copy(*texture);
        renderer->present();
    });

    client.set_key_event_callback([&](
        ice::engine& engine,
        const ice::key_event& key_event)
    {
        
    });


    // client.connect(host,port);
    client.init(ice::init_flags::ICE_INIT_VIDEO);
    client.loop();

    return 0;
}
