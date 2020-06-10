#include "ice-engine/network.h"
#include <iostream>

int main(const int argc, const char ** argv)
{
    ice::native_socket_t sock;
    struct sockaddr_in addr;
    std::shared_ptr<ice::ssl_context> secure_context;
    std::shared_ptr<ice::ssl_socket> secure_socket;
    const std::string msg = "Hello from the client";
    char msg_buf[100] = {0};

    #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif

    sock = socket(PF_INET,SOCK_STREAM,0);
    
    addr.sin_family = AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    addr.sin_port = htons(35000);

    connect(sock,reinterpret_cast<struct sockaddr*>(&addr),sizeof(addr));

    secure_context = std::make_shared<ice::ssl_context>(
        ice::CLIENT_TCP_SOCKET);

    secure_socket = std::make_shared<ice::ssl_socket>(
        *secure_context,
        sock);

    secure_socket->connect();

    if(secure_socket->write(
        reinterpret_cast<const uint8_t* const>(msg.data()),
        static_cast<uint32_t>(msg.size())) < 0)
    {
        throw std::runtime_error("Unable to write to socket");
    }

    std::cerr << "Client message sent" << std::endl;
    
    if(secure_socket->read(
        reinterpret_cast<uint8_t* const>(msg_buf),
        100) < 0)
    {
        throw std::runtime_error("Unable to read from socket");
    }

    std::cerr << "message from server: " << msg_buf << std::endl;

    std::cerr << "Hello" << std::endl;   
    #ifdef _WIN32
        WSACleanup();   
    #endif
    
    std::cerr << "Goodbye" << std::endl;

    return 0;
}