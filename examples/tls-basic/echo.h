#include <ice-engine/network.h>
#include <array>
#include <random>
#include <thread>
#include <vector>

#include <cstring>

namespace echo
{

enum command_codes
{
    ECH,
    CAP,
    SCR,
    REV
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
    uint32_t length);

void pack_response_header(
    response_header * const header, 
    uint8_t status,
    uint32_t length);

void read_request_header(
    ice::ssl_socket & client_socket, 
    request_header * const header);

void write_request_header(
    ice::ssl_socket & client_socket, 
    const request_header * const header);

void read_response_header(
    ice::ssl_socket & client_socket, 
    response_header * const header);

void write_response_header(
    ice::ssl_socket & client_socket, 
    const response_header * const header);

uint32_t read_request_body(
    ice::ssl_socket & client_socket, 
    uint8_t * const buffer, 
    uint32_t length);

uint32_t write_request_body(
    ice::ssl_socket & client_socket, 
    const uint8_t * const buffer, 
    uint32_t length);

uint32_t read_response_body(
    ice::ssl_socket & client_socket, 
    uint8_t * const buffer, 
    uint32_t length);

uint32_t write_response_body(
    ice::ssl_socket & client_socket, 
    const uint8_t * const buffer, 
    uint32_t length);

class client
{
private:
    std::string _host;
    uint16_t _port;
    ice::ssl_context _ctx;

    void send_request(
        const uint8_t command, 
        const uint8_t * const buffer, 
        const uint32_t length);

public:
    client(
        const std::string& host, 
        const uint16_t port);
    
    ~client() = default;

    void echo_message(
        const std::string& message);

    void capitalize_message(
        const std::string& message);

    void scramble_message(
        const std::string& message);

    void reverse_message(
        const std::string& message);
};

class server
{
private:
    ice::ssl_context _ctx;
    uint16_t _port;
    ice::native_socket_t _srv;
    std::default_random_engine _random_engine;
    uint32_t _threads;
    std::vector<std::thread> _thread_vector;
    bool _running;

    void handle_request(ice::ssl_socket & client_socket);

    void on_echo(
        ice::ssl_socket & client_socket, 
        uint8_t * const buffer, 
        const uint32_t length);

    void on_capitalize(
        ice::ssl_socket & client_socket, 
        uint8_t * const buffer, 
        const uint32_t length);

    void on_scramble(
        ice::ssl_socket & client_socket, 
        uint8_t * const buffer, 
        const uint32_t length);

    void on_reverse(
        ice::ssl_socket & client_socket, 
        uint8_t * const buffer, 
        const uint32_t length);

public:
    server(
        const std::string& cert_file, 
        const std::string& key_file, 
        const uint16_t port, 
        const uint32_t threads);

    ~server();

    void start();

    void run();
    
    void stop();
};
}