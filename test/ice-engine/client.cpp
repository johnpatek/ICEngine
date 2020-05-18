#include "ice-engine/network.h"
#include <iostream>
#include <cstring>

int main(const int argc, const char ** argv)
{
    ice::native_soket_t sock;
    struct sockaddr_in addr;
    std::shared_ptr<ice::ssl_context> secure_context;
    std::shared_ptr<ice::ssl_socket> secure_socket;
    const std::string msg = "Hello from the client";
    char msg_buf[100] = {0};

    sock = socket(PF_INET,SOCK_STREAM,0);
    
    addr.sin_family = AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    addr.sin_port = htons(35001);

    connect(sock,reinterpret_cast<struct sockaddr*>(&addr),sizeof(addr));

    secure_context = std::make_shared<ice::ssl_context>(
        ice::CLIENT_SOCKET);

    secure_socket = std::make_shared<ice::ssl_socket>(
        *secure_context,
        sock);

    secure_socket->connect();

    std::cerr << "Bytes written: " << secure_socket->write(
        reinterpret_cast<const uint8_t* const>(msg.data()),
        static_cast<uint32_t>(msg.size())) << std::endl;

    std::cerr << "Client message sent" << std::endl;
    
    std::cerr << "Bytes read: " << secure_socket->read(reinterpret_cast<uint8_t* const>(msg_buf),100) << std::endl;

    std::cerr << "message from server: " << msg_buf << std::endl;

    return 0;
}