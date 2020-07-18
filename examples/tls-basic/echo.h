#include <ice-engine/network.h>
#include <array>
#include <thread>
#include <vector>

namespace echo
{
    enum command_codes
    {
        ECHO,
        CAPS,
        
    };

    enum status_codes
    {
        OK,
        ERR
    };

    struct request_header
    {
        uint8_t command;
        uint8_t pad1;
        uint8_t pad2;
        uint8_t pad3;
        uint32_t length;
    };

    struct response_header
    {
        uint8_t status;
        uint8_t pad1;
        uint8_t pad2;
        uint8_t pad3;
        uint32_t length;
    };

    class client
    {
    private:
        std::string _host;
        uint16_t _port;
        std::shared_ptr<ice::ssl_context> _ctx;
    public:
        client(
            const std::string& host, 
            const uint16_t port);
        ~client();
    };

    class server
    {
    private:
        std::shared_ptr<ice::ssl_context> _ctx;
        uint16_t _port;
        uint32_t _thread_count;
        std::vector<std::thread> _threads;
    public:
        server(
            const std::string& key_file, 
            const std::string& cert_file, 
            const uint16_t port, 
            const uint32_t thread_count);

        ~server();

    };
}