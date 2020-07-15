#include "common.h"

ice::native_socket_t open_socket(
    int peer_type, 
    const char * const host, 
    uint16_t port)
{
    ice::native_socket_t result = socket(PF_INET,SOCK_STREAM,0);
    sockaddr_in addr;
    uint32_t len;

    if(result < 0)
    {
        throw std::runtime_error("Socket failed");
    }

    if(peer_type == CLIENT)
    {
        addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(host);
        addr.sin_port = htons(port);
        len = sizeof(addr);

        if(connect(
            result,
            reinterpret_cast<struct sockaddr*>(&addr),
            len) < 0)
        {
            throw std::runtime_error("Connect failed");    
        }
        
    }
    else if(peer_type == SERVER)
    {
        addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);
        len = sizeof(addr);

        if(bind(
            result,
            reinterpret_cast<sockaddr*>(&addr),
            len) < 0)
        {
            throw std::runtime_error("Bind failed");    
        }
    }
    else
    {
        // should be unreachable
    }

    return result;
}

void close_socket(ice::native_socket_t sock)
{
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}