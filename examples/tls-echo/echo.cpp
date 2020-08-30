#include "echo.h"

static void zero_memory(void * const data, const size_t size)
{
    memset(data,0,size);
}

enum peer_types 
{
    CLIENT,
    SERVER
};

static ice::native_socket_t open_socket(
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

static void close_socket(ice::native_socket_t sock)
{
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}

static size_t read_socket(
    ice::native_socket_t socket, 
    void * const data, 
    size_t size,
    const char * const error)
{
    ssize_t result;
#ifdef _WIN32
    result = _read(socket,data,size);
#else
    result = read(socket,data,size);
#endif
    if(result < 0)
    {
        throw std::runtime_error(error);
    }
    return static_cast<size_t>(result);
}

static size_t write_socket(
    ice::native_socket_t socket, 
    const void * const data, 
    size_t size,
    const char * const error)
{
    ssize_t result;
#ifdef _WIN32
    result = _write(socket,data,size);
#else
    result = write(socket,data,size);
#endif
    if(result < 0)
    {
        throw std::runtime_error(error);
    }
    return static_cast<size_t>(result);
}

void echo::pack_request_header(
    echo::request_header * const header, 
    int command, 
    size_t length)
{
    if(header != nullptr)
    {
        zero_memory(header,echo::REQUEST_HEADER_SIZE);
        header->command = static_cast<uint8_t>(command);
        header->length = static_cast<uint32_t>(length);
    }
}

void echo::unpack_request_header(
    const echo::request_header header, 
    int * const command, 
    size_t * const length)
{
    if(command != nullptr)
    {
        *command = static_cast<int>(header.command);
    }

    if(length != nullptr)
    {
        *length = static_cast<size_t>(header.length);
    }
}

void echo::read_request_header(
    ice::native_socket_t socket, 
    echo::request_header * const header)
{
    (void) read_socket(
        socket,
        header,
        echo::REQUEST_HEADER_SIZE,
        "Failed to read request header");
}

void echo::write_request_header(
    ice::native_socket_t socket, 
    const echo::request_header * const header)
{
    (void) write_socket(
        socket,
        header,
        echo::REQUEST_HEADER_SIZE,
        "Failed to write request header");
}

void echo::read_request_body(
    ice::native_socket_t socket,
    uint8_t * const data,
    const size_t size)
{
    (void) read_socket(
        socket,
        data,
        size,
        "Failed to read request body");
}

void echo::write_request_body(
    ice::native_socket_t socket,
    uint8_t * const data,
    const size_t size)
{
    (void) write_socket(
        socket,
        data,
        size,
        "Failed to write request body");
}

void echo::pack_response_header(
    echo::response_header * const header, 
    int status, 
    size_t length)
{
    if(header != nullptr)
    {
        zero_memory(header,echo::RESPONSE_HEADER_SIZE);
        header->status = static_cast<uint8_t>(status);
        header->length = static_cast<uint32_t>(length);
    }
}

void echo::unpack_response_header(
    const echo::response_header header, 
    int * const status, 
    size_t * const length)
{
    if(status != nullptr)
    {
        *status = static_cast<int>(header.status);
    }

    if(length != nullptr)
    {
        *length = static_cast<size_t>(header.length);
    }
}

void echo::read_response_header(
    ice::native_socket_t socket, 
    echo::response_header * const header)
{
    (void) read_socket(
        socket,
        header,
        echo::RESPONSE_HEADER_SIZE,
        "Failed to read response header");
}

void echo::write_response_header(
    ice::native_socket_t socket, 
    const response_header * const header)
{
    (void) write_socket(
        socket,
        header,
        echo::RESPONSE_HEADER_SIZE,
        "Failed to write response header");
}

void echo::read_response_body(
    ice::native_socket_t socket,
    uint8_t * const data,
    const size_t size)
{
    (void) read_socket(
        socket,
        data,
        size,
        "Failed to read response body");
}

void echo::write_response_body(
    ice::native_socket_t socket,
    uint8_t * const data,
    const size_t size)
{
    (void) write_socket(
        socket,
        data,
        size,
        "Failed to write response body");
}



void echo::server::start()
{
    if(listen(_listener,100) < 0)
    {
        throw std::runtime_error("Listen failed");
    }
    _running = true;
}

void echo::server::run(bool * const loop)
{
    ice::native_socket_t incoming_socket;
    std::pair<
        std::atomic<int>,
        ice::native_socket_t> incoming_connection;
    fd_set fds;
    ice::native_socket_t max_fd;
    while(*loop && _running)
    {
        FD_ZERO(&fds);
        FD_SET(_listener,&fds);
        max_fd = _listener;
        for(auto & connection : _connections)
        {
            if(connection.first.load() == echo::socket_states::READY)
            {
                FD_SET(connection.second,&fds);
                if(connection.second > max_fd)
                {
                    max_fd = connection.second;
                }
            }
        }

        if(select(max_fd + 1,&fds,nullptr,nullptr,nullptr) < 0)
        {
            throw std::runtime_error("Select failed");
        }

        if(FD_ISSET(_listener,&fds))
        {
            incoming_socket = accept(_listener,nullptr,nullptr);
            if(incoming_socket >= 0)
            {                
                incoming_connection.first.store(echo::socket_states::READY);
                incoming_connection.second = incoming_socket;
                _connections.push_back(incoming_connection);
            }
        }

        for(auto & connection : _connections)
        {
            if(connection.first.load() == echo::socket_states::READY)
            {
                if(FD_ISSET(connection.second,&fds))
                {
                    connection.first.store(echo::socket_states::BUSY);
                    _threadpool.call_async([&]
                    {
                        _server_callback(this,connection.first);
                        if(connection.first.load() == echo::socket_states::BUSY)
                        {
                            connection.first.store(echo::socket_states::READY);
                        }
                    });
                }
            }
        }

        for(auto & connection : _connections)
        {
            if(connection.first.load() == echo::socket_states::CLOSED)
            {
                close_socket(connection.second);
                _connections.remove(connection);
            }
        }

    }
}

void echo::server::stop()
{
    _running = false;
    close_socket(_listener);
    for(auto & connection : _connections)
    {
        if(connection.first.load() == echo::socket_states::CLOSED)
        {
            close_socket(connection.second);
            _connections.remove(connection);
        }
    }
}