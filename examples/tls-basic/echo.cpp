#include "echo.h"

static void zero_memory(uint8_t * const buffer, uint32_t length)
{
    (void)memset(buffer,0,length);
}

void echo::pack_request_header(
    echo::request_header * const header, 
    uint8_t command,
    uint32_t length)
{
    if(header == nullptr)
    {
        throw std::runtime_error("Request header is null");
    }

    zero_memory(reinterpret_cast<uint8_t* const>(header),
        echo::REQUEST_HEADER_SIZE);

    header->command = command;
    header->length = length;
}

void echo::pack_response_header(
    echo::response_header * const header, 
    uint8_t status,
    uint32_t length)
{
    if(header == nullptr)
    {
        throw std::runtime_error("Response header is null");
    }

    zero_memory(reinterpret_cast<uint8_t* const>(header),
        echo::RESPONSE_HEADER_SIZE);

    header->status = status;
    header->length = length;
}



void echo::read_request_header(
    ice::ssl_socket & client_socket, 
    echo::request_header * const header);

void echo::write_request_header(
    ice::ssl_socket & client_socket, 
    const echo::request_header * const client_header);

void echo::read_response_header(
    ice::ssl_socket & client_socket, 
    echo::response_header * const client_header);

void echo::write_response_header(
    ice::ssl_socket & socket, 
    const echo::response_header * const client_header);

uint32_t echo::read_request_body(
    ice::ssl_socket & client_socket, 
    uint8_t * const buffer, 
    uint32_t length);

uint32_t echo::read_response_body(
    ice::ssl_socket & client_socket, 
    uint8_t * const buffer, 
    uint32_t length);

uint32_t echo::write_request_body(
    ice::ssl_socket & client_socket, 
    const uint8_t * const buffer, 
    uint32_t length);

uint32_t echo::write_response_body(
    ice::ssl_socket & client_socket, 
    const uint8_t * const buffer, 
    uint32_t length);