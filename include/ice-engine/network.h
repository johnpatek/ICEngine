#ifndef NETWORK_H
#define NETWORK_H
#include "ice-engine/export.h"
#include <string>
#include <memory>
#if defined(_WIN32)
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#pragma comment(lib,"Ws2_32.lib")
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
    typedef SOCKET native_soket_t;
#else
    typedef int32_t native_soket_t;
#endif
    enum peer_types
    {
        CLIENT_SOCKET,
        SERVER_SOCKET
    };
    
    class ssl_context
    {
    private:
        std::shared_ptr<void> _ctx;
    public:
        API_EXPORT ssl_context(int peer_type);
        
        API_EXPORT ssl_context(
            int peer_type, 
            const std::string& cert_file, 
            const std::string& key_file);
        
        API_EXPORT ~ssl_context() = default;

        API_EXPORT void * data() const;
    };
    
    class ssl_socket
    {
    private:
        std::shared_ptr<void> _ssl;
    public:
        API_EXPORT ssl_socket(
             const ssl_context & ctx, 
             native_soket_t desc);
        
        API_EXPORT ssl_socket(const ssl_socket& s) = default;

        API_EXPORT ssl_socket(ssl_socket&& s) = default;

        API_EXPORT ~ssl_socket() = default;

        API_EXPORT void accept();

        API_EXPORT void connect();

        API_EXPORT int32_t read(
            uint8_t * const data, 
            uint32_t size);

        API_EXPORT int32_t write(
            const uint8_t * const data, 
            uint32_t size);
    };
}
#endif