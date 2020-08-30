#ifndef ECHO_H
#define ECHO_H
#include <ICEngine.h>
#include <cstring>
#include <random>
#ifdef _WIN32
#else
#include <signal.h>
#include <unistd.h>
#endif
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

const size_t REQUEST_HEADER_SIZE = sizeof(request_header);

const size_t RESPONSE_HEADER_SIZE = sizeof(response_header);

void pack_request_header(
    request_header * const header, 
    int command, 
    size_t length);

void unpack_request_header(
    const request_header header, 
    int * const command, 
    size_t * const length);

void read_request_header(
    ice::native_socket_t socket, 
    request_header * const header);

void write_request_header(
    ice::native_socket_t socket, 
    const request_header * const header);

void read_request_body(
    ice::native_socket_t socket,
    uint8_t * const data,
    const size_t size);

void write_request_body(
    ice::native_socket_t socket,
    uint8_t * const data,
    const size_t size);

void pack_response_header(
    response_header * const header, 
    int status, 
    size_t length);

void unpack_response_header(
    const response_header header, 
    int * const status, 
    size_t * const length);

void read_response_header(
    ice::native_socket_t socket, 
    response_header * const header);

void write_response_header(
    ice::native_socket_t socket, 
    const response_header * const header);

void read_response_body(
    ice::native_socket_t socket,
    uint8_t * const data,
    const size_t size);

void write_response_body(
    ice::native_socket_t socket,
    uint8_t * const data,
    const size_t size);


enum socket_states 
{
    READY,BUSY,CLOSED
};

class server
{
private:
    ice::native_socket_t _listener;
    ice::threadpool _threadpool;
    std::list<std::pair<std::atomic<int>,ice::native_socket_t>> _connections;
    std::mutex _connections_mutex;
    std::default_random_engine _random_engine;
    server_callback_t _server_callback;
    bool _running;
public:
    server(
        uint16_t port,
        const size_t threads,
        const server_callback_t& server_callback);

    void start();

    void run(bool * const loop);

    void stop();

    void close_connection(const ice::native_socket_t socket);

    std::default_random_engine& random_engine();
};

typedef std::function<void(server * const,ice::native_socket_t)> server_callback_t;

}

#endif