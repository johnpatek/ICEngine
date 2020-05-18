#include "ice-engine/network.h"
#include <iostream>

void serve(const int argc, const char ** argv)
{
    ice::native_soket_t srv,cli;
    struct sockaddr_in addr;
    std::string cert_path, key_path;
    std::shared_ptr<ice::ssl_context> secure_context;
    std::shared_ptr<ice::ssl_socket> secure_socket;    
    const char * const msg = "Hello from the server";
    char msg_buf[100];


    if(argc == 3)
    {
        cert_path = argv[1];
        key_path = argv[2];
        secure_context = std::make_shared<ice::ssl_context>(
            ice::SERVER_SOCKET,
            cert_path,
            key_path);

        srv = socket(PF_INET,SOCK_STREAM,0);

        if(srv < 0)
        {
            throw std::runtime_error("Unable to create socket");
        }

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(35000);

        if(bind(srv,reinterpret_cast<struct sockaddr*>(&addr),sizeof(addr)) < 0)
        {
            throw std::runtime_error("Unable to bind socket");    
        }

        if(listen(srv,3) < 0)
        {
            throw std::runtime_error("Unable to listen");
        }

        cli = accept(srv,nullptr,nullptr);

        if(cli < 0)
        {
            throw std::runtime_error("Unable to accept");    
        }

        secure_socket = std::make_shared<ice::ssl_socket>(*secure_context,cli);
        
        secure_socket->read(reinterpret_cast<uint8_t* const>(msg_buf),100);

        std::cerr << "Message from client: " << msg_buf << std::endl;

        secure_socket->write(
            reinterpret_cast<const uint8_t* const>(msg),
            static_cast<uint32_t>(strlen(msg) + 1));

        std::cerr << "Server message sent" << std::endl;

        #ifdef _WIN32
        _close(static_cast<int>(srv));
        #else
        close(srv);
        #endif
    }
}

int main(const int argc, const char ** argv)
{
    try
    {
        serve(argc,argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}