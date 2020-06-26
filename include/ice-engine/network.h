#ifndef NETWORK_H
#define NETWORK_H
#include <string>
#include <memory>
#include <iostream>
#if defined(_WIN32)
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#pragma  comment(lib, "ws2_32.lib ")
#else /* Unix */
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#endif

namespace ice
{
#if defined(_WIN32)
    typedef SOCKET native_socket_t;
#else
    typedef int32_t native_socket_t;
#endif
    enum peer_types
    {
        CLIENT_TCP_SOCKET,
        SERVER_TCP_SOCKET,
        CLIENT_UDP_SOCKET,
        SERVER_UDP_SOCKET
    };

    class ssl_context
    {
    private:
        std::shared_ptr<void> _ctx;
    public:
        ssl_context(int peer_type);
        
        ssl_context(
            int peer_type, 
            const std::string& cert_file, 
            const std::string& key_file);
        
        ssl_context(const ssl_context& o) = default;

        ssl_context(ssl_context&& o) = default;

        ~ssl_context() = default;

        void * data() const;
    };
    
    class ssl_socket
    {
    private:
        std::shared_ptr<void> _ssl;
    public:
        ssl_socket(
             const ssl_context & ctx, 
             native_socket_t desc);
        
        ssl_socket(const ssl_socket& s) = default;

        ssl_socket(ssl_socket&& s) = default;

        ~ssl_socket() = default;

        int32_t accept();

        int32_t connect();

        int32_t read(
            uint8_t * const data, 
            uint32_t size);

        int32_t write(
            const uint8_t * const data, 
            uint32_t size);
    };
    

}
#endif