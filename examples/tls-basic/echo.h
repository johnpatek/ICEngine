#include <ice-engine/network.h>
#include <array>
#include <thread>
#include <vector>

namespace echo
{
enum command_codes
{
    ECHO,
    CAPITALIZE,
    SCRAMBLE,
    REVERSE
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

const uint32_t REQUEST_HEADER_SIZE = sizeof(request_header);
const uint32_t RESPONSE_HEADER_SIZE = sizeof(response_header);

void pack_request_header(
    request_header * const header, 
    uint8_t command, 
    uint32_t timestamp, 
    uint32_t length);

void pack_response_header(
    response_header * const header, 
    uint8_t status,
    uint32_t length);

void read_request_header(
    ice::ssl_socket & socket, 
    request_header * const header);

void write_request_header(
    ice::ssl_socket & socket, 
    const request_header * const header);

void read_response_header(
    ice::ssl_socket & socket, 
    response_header * const header);

void write_response_header(
    ice::ssl_socket & socket, 
    const response_header * const header);

uint32_t read_request_body(
    ice::ssl_socket & socket, 
    uint8_t * const buffer, 
    uint32_t length);

uint32_t read_response_body(
    ice::ssl_socket & socket, 
    uint8_t * const buffer, 
    uint32_t length);

uint32_t write_request_body(
    ice::ssl_socket & socket, 
    const uint8_t * const buffer, 
    uint32_t length);

uint32_t write_response_body(
    ice::ssl_socket & socket, 
    const uint8_t * const buffer, 
    uint32_t length);

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

    void echo_message(const std::string& message);

    void capitalize_message(const std::string& message);

    void scramble_message(const std::string& message);

    void reverse_message(const std::string& message);
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