#include "ice-engine/argparse.h"
#include "ice-engine/network.h"
#include <array>

const uint32_t BUFFER_SIZE = 1024;
const std::string SERVER_MESSAGE("Message from the server.\n");

void server_main(
    const std::string& cert,
    const std::string& key,
    const uint16_t port);

int main(const int argc, const char ** argv)
{
    ice::argument_parser parser("tcp-server","A simple tcp server example.");
    parser.add_argument("-p","--port","port",true);
    parser.add_argument("-c","--cert","cert",true);
    parser.add_argument("-k","--key","key",true);
    parser.enable_help();

    if(!parser.parse(argc,argv) 
        && parser.exists("port") 
        && parser.exists("key") 
        && parser.exists("cert"))
    {
        try
        {
            server_main(
                parser.get<std::string>("cert"),
                parser.get<std::string>("key"),
                parser.get<uint16_t>("port"));
        }
        catch(const std::exception& e)
        {
            std::cerr << "Server Fault: " << e.what() << std::endl;
        }
            
    }
    else
    {
        parser.print_help();
    }

    return 0;
}

void server_main(
    const std::string& cert,
    const std::string& key,
    const uint16_t port)
{
    ice::native_socket_t srv,cli;
    struct sockaddr_in srvaddr,cliaddr;
    socklen_t srvlen,clilen;
    ice::ssl_context ctx(
        ice::SERVER_TCP_SOCKET,
        cert,
        key);

    std::array<uint8_t,BUFFER_SIZE> buffer;

    srv = socket(PF_INET,SOCK_STREAM,0);

    if(srv < 0)
    {
        throw std::runtime_error("Bad socket");
    }

    srvaddr = {0};
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port = htons(port);
    srvlen = sizeof(srvaddr);

    if(bind(
        srv,
        reinterpret_cast<struct sockaddr*>(&srvaddr),
        srvlen) < 0)
    {
        throw std::runtime_error("Bind failed");    
    }

    if(listen(srv,1) < 0)
    {
        throw std::runtime_error("Listen failed");    
    }

    cliaddr = {0};
    clilen = 0;
    cli = accept(
        srv,
        reinterpret_cast<struct sockaddr*>(&cliaddr),
        &clilen);

    if(cli < 0)
    {
        throw std::runtime_error("Accept failed");
    }

    ice::ssl_socket sock(ctx,cli);

    if(sock.accept() < 0)
    {
        throw std::runtime_error("SSL accept failed");    
    }

    std::cerr << "Connection Accepted"
              << std::endl
              << inet_ntoa(cliaddr.sin_addr) 
              << ":"
              << ntohs(cliaddr.sin_port)
              << std::endl;
    
    uint32_t read_size = sock.read(buffer.data(),buffer.size());

    if(read_size < 0)
    {
        throw std::runtime_error("Read failed");
    }
    
    std::cerr << "Message received: ";

    std::cerr.write(
        reinterpret_cast<const char* const>(
            buffer.data()),
            read_size);

    std::cerr << std::endl;

    if(sock.write(
        reinterpret_cast<const uint8_t* const>(
            SERVER_MESSAGE.data()),
        SERVER_MESSAGE.size()) < 0)
    {
        throw std::runtime_error("Write failed");
    }

    std::cerr << "Message sent." << std::endl;

    close(srv);
}