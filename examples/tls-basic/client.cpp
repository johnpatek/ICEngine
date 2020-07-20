#include "ice-engine/argparse.h"
#include "echo.h"

static void client_main(
    const std::string& address, 
    const uint16_t port);

int main(const int argc, const char ** argv)
{
    ice::argument_parser parser("tcp-client","A simple tcp client example.");
    parser.add_argument("-a","--address","address",true);
    parser.add_argument("-p","--port","port",true);
    parser.enable_help();

    if(!parser.parse(argc,argv) 
        && parser.exists("address")
        && parser.exists("port"))
    {
        try
        {
            client_main(
                parser.get<std::string>("address"),
                parser.get<uint16_t>("port"));
        }
        catch(const std::exception& e)
        {
            std::cerr << "Client Error: " << e.what() << std::endl;
        }
    }
    else
    {
        parser.print_help();
    }

    return 0;
}

static void prompt_message(std::string& buffer);

static void client_main(
    const std::string& address, 
    const uint16_t port)
{
    echo::client client(address,port);
    std::string command_buffer, message_buffer;
    bool client_loop(true);


}


static void prompt_message(std::string buffer)
{
    
}