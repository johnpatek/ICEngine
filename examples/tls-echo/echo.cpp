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

static uint32_t read_socket(
    ice::ssl_socket & client_socket,
    uint8_t * const buffer,
    uint32_t length,
    const char * const error_message)
{
    int32_t result;
    result = client_socket.read(buffer,length);
    if(result < 0)
    {
        throw std::runtime_error(error_message);
    }
    return static_cast<uint32_t>(result);
}

static uint32_t write_socket(
    ice::ssl_socket & client_socket,
    const uint8_t * const buffer,
    uint32_t length,
    const char * const error_message)
{
    int32_t result;
    result = client_socket.write(buffer,length);
    if(result < 0)
    {
        throw std::runtime_error(error_message);
    }
    return static_cast<uint32_t>(result);
}

void echo::read_request_header(
    ice::ssl_socket & client_socket, 
    echo::request_header * const header)
{
    (void) read_socket(
        client_socket,
        reinterpret_cast<uint8_t* const>(header),
        echo::REQUEST_HEADER_SIZE,
        "Failed to read request header");
}

void echo::write_request_header(
    ice::ssl_socket & client_socket, 
    const echo::request_header * const header)
{
    (void) write_socket(
        client_socket,
        reinterpret_cast<const uint8_t* const>(header),
        echo::REQUEST_HEADER_SIZE,
        "Failed to write request header");
}

void echo::read_response_header(
    ice::ssl_socket & client_socket, 
    echo::response_header * const header)
{
    (void) read_socket(
        client_socket,
        reinterpret_cast<uint8_t* const>(header),
        echo::REQUEST_HEADER_SIZE,
        "Failed to read response header");
}

void echo::write_response_header(
    ice::ssl_socket & client_socket, 
    const echo::response_header * const header)
{
    (void) write_socket(
        client_socket,
        reinterpret_cast<const uint8_t* const>(header),
        echo::REQUEST_HEADER_SIZE,
        "Failed to write response header");
}

uint32_t echo::read_request_body(
    ice::ssl_socket & client_socket, 
    uint8_t * const buffer, 
    uint32_t length)
{
    return read_socket(
        client_socket,
        buffer,
        length,
        "Failed to read request body");
}

uint32_t echo::write_request_body(
    ice::ssl_socket & client_socket, 
    const uint8_t * const buffer, 
    uint32_t length)
{
    return write_socket(
        client_socket,
        buffer,
        length,
        "Failed to write request body");
}

uint32_t echo::read_response_body(
    ice::ssl_socket & client_socket, 
    uint8_t * const buffer, 
    uint32_t length)
{
    return read_socket(
        client_socket,
        buffer,
        length,
        "Failed to read response body");
}

uint32_t echo::write_response_body(
    ice::ssl_socket & client_socket, 
    const uint8_t * const buffer, 
    uint32_t length)
{
    return write_socket(
        client_socket,
        buffer,
        length,
        "Failed to write response body");
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


echo::client::client(
    const std::string& host, 
    const uint16_t port) : _ctx(ice::CLIENT_TCP_SOCKET)
{
    _host = host;
    _port = port;
}

void echo::client::send_request(
    const uint8_t command, 
    const uint8_t * const buffer, 
    const uint32_t length)
{
    ice::native_socket_t native_socket = open_socket(
        CLIENT,
        _host.c_str(),
        _port);

    ice::ssl_socket client_socket(
        _ctx,native_socket);

    echo::request_header request_header;
    echo::response_header response_header;
    std::vector<uint8_t> response_body;
    uint32_t response_length;

    echo::pack_request_header(&request_header,command,length);

    client_socket.connect();

    echo::write_request_header(client_socket,&request_header);

    echo::write_request_body(client_socket,buffer,length);

    echo::read_response_header(client_socket,&response_header);

    response_length = response_header.length;

    if(response_length > 0)
    {
        response_body.resize(response_length);

        echo::read_response_body(
            client_socket,
            response_body.data(),
            response_length);
        
        std::cerr.write(
            reinterpret_cast<const char*>(response_body.data()),
            response_length);
        
        std::cerr << std::endl;
    }
}

void echo::client::echo_message(
    const std::string& message)
{
    send_request(
        echo::command_codes::ECH,
        reinterpret_cast<const uint8_t * const>(message.data()),
        static_cast<uint32_t>(message.length()));
}

void echo::client::capitalize_message(
    const std::string& message)
{
    send_request(
        echo::command_codes::CAP,
        reinterpret_cast<const uint8_t * const>(message.data()),
        static_cast<uint32_t>(message.length()));
}

void echo::client::scramble_message(
    const std::string& message)
{
    send_request(
        echo::command_codes::SCR,
        reinterpret_cast<const uint8_t * const>(message.data()),
        static_cast<uint32_t>(message.length()));
}

void echo::client::reverse_message(
    const std::string& message)
{
    send_request(
        echo::command_codes::REV,
        reinterpret_cast<const uint8_t * const>(message.data()),
        static_cast<uint32_t>(message.length()));
}

static void error_response(
    ice::ssl_socket & client_socket, 
    const std::string& error_message);

void echo::server::handle_request(ice::ssl_socket & client_socket)
{
    echo::request_header request_header;
    echo::response_header response_header;
    std::vector<uint8_t> request_body;
    uint8_t response_status;
    uint8_t * response_buffer;
    uint32_t request_length, response_length;
    std::string error_message;
    bool error(false);

    echo::read_request_header(client_socket,&request_header);

    response_status = echo::status_codes::OK;

    request_length = request_header.length;

    if(request_length > 0)
    {
        request_body.resize(request_length);
        
        echo::read_request_body(
            client_socket,
            request_body.data(),
            request_length);
        
        response_length = request_length;

        switch(request_header.command)
        {
            case echo::command_codes::ECH:
                response_buffer = request_body.data();
                break;
            case echo::command_codes::CAP:
                for(uint8_t& byte : request_body)
                {
                    if((byte >= (uint8_t)'a') 
                        || (byte <= (uint8_t)'z'))
                    {
                        byte -= ('a' - 'A');
                    }
                }
                response_buffer = request_body.data();
                break;
            case echo::command_codes::SCR:
                std::shuffle(
                    request_body.begin(),
                    request_body.end(),
                    _random_engine);
                response_buffer = request_body.data();
                break;
            case echo::command_codes::REV:
                std::reverse(
                    request_body.begin(),
                    request_body.end());
                response_buffer = request_body.data();
                break;
            default:
                error_message = "Invalid command code";
                error = true;
        }
    }
    else
    {
        error_message = "Empty request";
        error = true;
    }

    if(!error)
    {
        echo::pack_response_header(
            &response_header,
            response_status,
            response_length);

        echo::write_response_header(client_socket,&response_header);

        echo::write_response_body(
            client_socket,
            response_buffer,
            response_length);
    }
    else
    {
        error_response(client_socket,error_message);
    }
}

static void error_response(
    ice::ssl_socket & client_socket, 
    const std::string& error_message)
{
    echo::response_header response_header;
    uint32_t response_length;

    response_length = static_cast<uint32_t>(
        error_message.length());

    echo::pack_response_header(
        &response_header, 
        echo::status_codes::ERR, 
        response_length);

    echo::write_response_header(client_socket,&response_header);

    echo::write_response_body(
        client_socket,
        reinterpret_cast<const uint8_t* const>(error_message.data()),
        response_length);
}

echo::server::server(
    const std::string& cert_file, 
    const std::string& key_file, 
    const uint16_t port, 
    const uint32_t threads) : _ctx(
        ice::SERVER_TCP_SOCKET,
        cert_file,key_file), _random_engine(
        std::random_device()())
{
    _port = port;
    _threads = threads;
    _srv = open_socket(SERVER,nullptr,_port);
}

echo::server::~server()
{
    if(_running)
    {
        stop();
    }
}

void echo::server::start()
{
    if(listen(_srv, _threads * 2) < 0)
    {
        throw std::runtime_error("Failed to listen on socket");
    }
    while(_thread_vector.size() < _threads)
    {
        _thread_vector.push_back(std::thread([this]
        {
            this->run();
        }));
    }
}

void echo::server::run()
{
    ice::native_socket_t native_socket;
    bool loop(true);
    while(loop)
    {
        native_socket = accept(_srv,nullptr,nullptr);
        if(native_socket >= 0)
        {
            ice::ssl_socket client_socket(_ctx,native_socket);
            client_socket.accept();
            handle_request(client_socket);
        }
        else
        {
            loop = false;
        }
    }
}

void echo::server::stop()
{
    close_socket(_srv);
    for(std::thread & thread : _thread_vector)
    {
        thread.join();
    }
    _running = false;
}