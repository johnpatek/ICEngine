#include "ice-engine/network.h"
#include <array>
#include <fstream>
#include<mutex>
#include <signal.h>
#include <thread>
#include <shared_mutex>
#include <cstring>

namespace common
{

const uint32_t BUFFER_SIZE = 1024;

void zero_memory(uint8_t * const buf, uint32_t len);

uint32_t timestamp();

uint32_t file_size(const std::string path);

struct request_header
{
    uint8_t command;
    uint8_t pad1;
    uint8_t pad2;
    uint8_t pad3;
    uint32_t timestamp;
    uint32_t length;
};

const uint32_t REQUEST_HEADER_SIZE = sizeof(request_header);

struct response_header
{
    uint8_t status;
    uint8_t pad1;
    uint8_t pad2;
    uint8_t pad3;
    uint32_t length;
};

const uint32_t RESPONSE_HEADER_SIZE = sizeof(response_header);

enum command_codes
{
    LOG, DUMP
};

enum status_codes
{
    OK,ERR
};

enum peer_types
{
    CLIENT, SERVER
};

void pack_request_header(
    request_header * const header, 
    uint8_t command, 
    uint32_t timestamp, 
    uint32_t length);

void unpack_request_header(
    request_header header, 
    uint8_t * const command, 
    uint32_t * const timestamp, 
    uint32_t * const length);

void pack_response_header(
    response_header * const header, 
    uint8_t status,
    uint32_t length);

void unpack_response_header(
    response_header header, 
    uint8_t * const status, 
    uint32_t * const length);


ice::native_socket_t open_socket(
    int peer_type, 
    const char * const host, 
    uint16_t port);

void close_socket(ice::native_socket_t sock);

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
}