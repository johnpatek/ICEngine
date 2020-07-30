#include "common.h"

void common::zero_memory(uint8_t * const buf, uint32_t len)
{
    memset(buf,0,len);
}

uint32_t common::timestamp()
{
    return static_cast<uint32_t>(
        std::chrono::system_clock::now().time_since_epoch().count());
}

uint32_t common::file_size(const std::string path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    return static_cast<uint32_t>(file.tellg());
}

void common::pack_request_header(
    common::request_header * const header, 
    uint8_t command, 
    uint32_t timestamp, 
    uint32_t length)
{
    if(header != nullptr)
    {
        common::zero_memory(
            reinterpret_cast<uint8_t*>(header),
            REQUEST_HEADER_SIZE);
        header->command = command;
        header->timestamp = timestamp;
        header->length = length;
    }
}

void common::unpack_request_header(
    common::request_header header, 
    uint8_t * const command, 
    uint32_t * const timestamp, 
    uint32_t * const length)
{
    if((command != nullptr) && (timestamp != nullptr) && (length != nullptr))
    {
        *command = header.command;
        *timestamp = header.timestamp;
        *length = header.length;
    }
}

void common::pack_response_header(
    common::response_header * const header, 
    uint8_t status,
    uint32_t length)
{
    if(header != nullptr)
    {
        zero_memory(
            reinterpret_cast<uint8_t*>(header),
            RESPONSE_HEADER_SIZE);
        header->status = status;
        header->length = length;
    }
}

void common::unpack_response_header(
    common::response_header header, 
    uint8_t * const status, 
    uint32_t * const length)
{
    if((status != nullptr) && (length != nullptr))
    {
        *status = header.status;
        *length = header.length;
    }
}

void common::read_request_header(
    ice::ssl_socket & socket, 
    common::request_header * const header)
{
    if(socket.read(
        reinterpret_cast<uint8_t* const>(header),
        common::REQUEST_HEADER_SIZE) < 0)
    {
        throw std::runtime_error("Failed to read request header");
    }
}

void common::write_request_header(
    ice::ssl_socket & socket, 
    const common::request_header * const header)
{
    if(socket.write(
        reinterpret_cast<const uint8_t* const>(header),
        common::REQUEST_HEADER_SIZE) < 0)
    {
        throw std::runtime_error("Failed to write request header");
    }
}

void common::read_response_header(
    ice::ssl_socket & socket, 
    common::response_header * const header)
{
    if(socket.read(
        reinterpret_cast<uint8_t* const>(header),
        common::RESPONSE_HEADER_SIZE) < 0)
    {
        throw std::runtime_error("Failed to read response header");    
    }
}

void common::write_response_header(
    ice::ssl_socket & socket, 
    const common::response_header * const header)
{
    if(socket.write(reinterpret_cast<const uint8_t * const>(
        header),common::RESPONSE_HEADER_SIZE) < 0)
    {
        throw std::runtime_error("Failed to write response header");
    }
}


ice::native_socket_t common::open_socket(
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
        inet_pton(AF_INET,host,&addr.sin_addr.s_addr);
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

void common::close_socket(ice::native_socket_t sock)
{
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}


static uint32_t read_body(
    ice::ssl_socket & socket, 
    uint8_t * const buffer, 
    uint32_t length, 
    const std::string& error_message);

static uint32_t write_body(
    ice::ssl_socket & socket, 
    const uint8_t * const buffer, 
    uint32_t length,
    const std::string& error_message);

uint32_t common::read_request_body(
    ice::ssl_socket & socket, 
    uint8_t * const buffer, 
    uint32_t length)
{
    return read_body(socket,buffer,length,
        "Failed to read request body");
}

uint32_t common::read_response_body(
    ice::ssl_socket & socket, 
    uint8_t * const buffer, 
    uint32_t length)
{
    return read_body(socket,buffer,length,
        "Failed to read response body");
}

uint32_t common::write_request_body(
    ice::ssl_socket & socket, 
    const uint8_t * const buffer, 
    uint32_t length)
{
    return write_body(socket,buffer,length,
        "Failed to write request body");
}

uint32_t common::write_response_body(
    ice::ssl_socket & socket, 
    const uint8_t * const buffer, 
    uint32_t length)
{
    return write_body(socket,buffer,length,
        "Failed to write response body");
}

static uint32_t read_body(
    ice::ssl_socket & socket, 
    uint8_t * const buffer, 
    uint32_t length,
    const std::string& error_message)
{
    int32_t result = socket.read(buffer,length);
    if(result < 0)
    {
        throw std::runtime_error(error_message.c_str());
    }
    return static_cast<uint32_t>(result);
}

static uint32_t write_body(
    ice::ssl_socket & socket, 
    const uint8_t * const buffer, 
    uint32_t length,
    const std::string& error_message)
{
    int32_t result = socket.write(buffer,length);
    if(result < 0)
    {
        throw std::runtime_error(error_message.c_str());
    }
    return static_cast<uint32_t>(result);
}