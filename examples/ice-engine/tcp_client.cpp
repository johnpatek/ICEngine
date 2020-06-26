#include "ice-engine/argparse.h"
#include "ice-engine/network.h"
#include <array>

const uint32_t BUFFER_SIZE = 1024;
const std::string CLIENT_MESSAGE("Message from client\n");

void client_main(
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

void client_main(
    const std::string& address, 
    const uint16_t port)
{
    ice::native_socket_t cli;
    struct sockaddr_in addr;
    socklen_t len;
    ice::ssl_context ctx(ice::CLIENT_TCP_SOCKET);
    std::array<uint8_t,BUFFER_SIZE> buffer;

    cli = socket(PF_INET,SOCK_STREAM,0);

    if(cli < 0)
    {
        throw std::runtime_error("Socket failed");
    }

    addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address.c_str());
    addr.sin_port = htons(port);
    len = sizeof(addr);

    if(connect(cli,reinterpret_cast<struct sockaddr*>(&addr),len) < 0)
    {
        throw std::runtime_error("Connect failed");    
    }

    ice::ssl_socket sock(ctx,cli);

    if(sock.connect() < 0)
    {
        throw std::runtime_error("SSL Connect failed");
    }

    if(sock.write(
        reinterpret_cast<const uint8_t* const>(
            CLIENT_MESSAGE.data()),
        CLIENT_MESSAGE.size()) < 0)
    {    
        throw std::runtime_error("Write failed");
    }

    std::cerr << "Message sent." << std::endl;

    uint32_t read_size = sock.read(
        buffer.data(),
        buffer.size());

    if(read_size < 0)
    {    
        throw std::runtime_error("Read failed");
    }

    std::cerr << "Message recieved: ";

    std::cerr.write(
        reinterpret_cast<const char* const>(
            buffer.data()),
        read_size);

    std::cerr << std::endl;
}