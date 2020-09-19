#include "netpong.h"




int main(const int argc, const char ** argv)
{
    ice::client client(8);
    std::shared_ptr<ice::window> window;
    std::shared_ptr<ice::renderer> renderer;
    ice::resource paddle;
    std::string host;
    uint16_t port;
    int speed = 0;
    int current_x;

    client.set_init_function([&](ice::engine& engine)
    {
        window = std::make_shared<ice::window>(
            "netpong",
            ice::position_flags::ICE_POSITION_CENTERED,
            ice::position_flags::ICE_POSITION_CENTERED,
            640,
            480);
        client.create_resource("player_paddle",paddle);
    });

    client.set_loop_function([&](ice::engine& engine)
    {
        current_x = paddle.get_property("x");
        paddle.set_property("x", current_x + speed);
        client.update_resource("player_paddle",paddle);
    });

    client.set_key_event_callback([&](
        ice::engine& engine,
        const ice::key_event& key_event)
    {
        if(key_event.get_event_type() == ice::key_event_types::ICE_KEY_PRESSED 
            && key_event.get_key_code() == ice::key_codes::ICE_KEY_CODE_LEFT
            && !key_event.get_repeat())
        {
            speed = -1;
        }
        else if(key_event.get_event_type() == ice::key_event_types::ICE_KEY_PRESSED 
            && key_event.get_key_code() == ice::key_codes::ICE_KEY_CODE_RIGHT
            && !key_event.get_repeat())
        {
            speed = 1;
        }
        else
        {
            speed = 0;
        }
    });


    // client.connect(host,port);
    client.init(ice::init_flags::ICE_INIT_VIDEO);


    return 0;
}
