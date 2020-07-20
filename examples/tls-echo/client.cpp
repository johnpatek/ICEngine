#include "echo.h"
#include <ice-engine/argparse.h>

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

static void print_help();

static void client_main(
    const std::string& address, 
    const uint16_t port)
{
    echo::client client(address,port);
    std::string command_buffer, message_buffer;
    bool loop(true);

#ifdef _WIN32    
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2),&data);
#endif

    while(loop)
    {
        std::cerr << ">>>";
        std::getline(std::cin,command_buffer);

        if(command_buffer == "ECH" || command_buffer == "ech")
        {
            prompt_message(message_buffer);
            client.echo_message(message_buffer);
        }
        else if(command_buffer == "CAP" || command_buffer == "cap")
        {
            prompt_message(message_buffer);
            client.capitalize_message(message_buffer);
        }
        else if(command_buffer == "SCR" || command_buffer == "scr")
        {
            prompt_message(message_buffer);
            client.scramble_message(message_buffer);

        }
        else if(command_buffer == "REV" || command_buffer == "rev")
        {
            prompt_message(message_buffer);
            client.reverse_message(message_buffer);

        }
        else if(command_buffer == "QUIT" || command_buffer == "quit")
        {
            loop = false;
        }
        else if(command_buffer == "HELP" || command_buffer == "help")
        {
            print_help();   
        }
        else
        {
            std::cerr << "Unrecognized command \"" 
                    << command_buffer 
                    << "\", enter \"HELP\" for a list of commands."
                    << std::endl;
        }
    }
#ifdef _WIN32
    WSACleanup();
#endif
}


static void prompt_message(std::string& buffer)
{
    std::cerr << "enter message: ";
    std::getline(std::cin,buffer);
}

static void print_help()
{
    std::cerr << "ECH - Echo a message" << std::endl
              << "CAP - Capitalize a message" << std::endl
              << "SCR - Scramble a message" << std::endl
              << "REV - Reverse a message" << std::endl
              << "QUIT - Quit the client application" << std::endl
              << "HELP - Print this message" << std::endl;
}