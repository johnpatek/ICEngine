#include "netpong.h"

int main(const int argc, const char ** argv)
{
    ice::client client(8);


    client.set_key_event_callback([&client](
        ice::engine& engine,
        const ice::key_event& key_event)
    {

    });

    

    return 0;
}
