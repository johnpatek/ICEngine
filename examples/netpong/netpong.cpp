#include "netpong.h"

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

static ssize_t read_socket(
    ice::native_socket_t socket,
    void * const data,
    const size_t size)
{
    ssize_t result;
#ifdef _WIN32
    result = _read(socket,data,size);
#else
    result = read(socket,data,size);
#endif
    return result;
}

static ssize_t write_socket(
    ice::native_socket_t socket,
    const void * const data,
    const size_t size)
{
    ssize_t result;
#ifdef _WIN32
    result = _write(socket,data,size);
#else
    result = write(socket,data,size);
#endif
    return result;
}

void netpong::pack_message_header(
    netpong::message_header * const header, 
    int type, 
    size_t size)
{
    if(header != nullptr)
    {
        header->type = type;
        header->length = size;
    }
}

void netpong::unpack_message_header(
    const netpong::message_header& header, 
    int * const type, 
    size_t * const size)
{
    if(type != nullptr)
    {
        *type = header.type;
    }

    if(size != nullptr)
    {
        *size = header.length;
    }
}

ssize_t netpong::read_message_header(
    ice::native_socket_t socket, 
    netpong::message_header * const header)
{
    return read_socket(
        socket,
        header,
        netpong::MESSAGE_HEADER_SIZE);
}

ssize_t netpong::write_message_header(
    ice::native_socket_t socket, 
    const netpong::message_header * const header)
{
    return write_socket(
        socket,
        header,
        netpong::MESSAGE_HEADER_SIZE);
}

ssize_t netpong::read_message_body(
    ice::native_socket_t socket,
    void * const data,
    const size_t size)
{
    return read_socket(socket,data,size);
}

ssize_t netpong::write_message_body(
    ice::native_socket_t socket,
    const void * const data,
    const size_t size)
{
    return write_socket(socket,data,size);
}

netpong::client::client(const std::string& host,const uint16_t port)
{
    _quit = false;
    try
    {    
        _socket = open_socket(
            peer_types::CLIENT,
            host.c_str(),
            port);
    }
    catch(const std::exception& e)
    {
        _quit = true;
    }
}


void netpong::client::main_loop()
{
    std::string line_buffer,type_string,message_string;
    std::shared_ptr<netpong::message_header> shared_header;
    std::shared_ptr<std::vector<uint8_t>> shared_buffer;
    fd_set read_set, write_set;
    int max_fd;
    int type;
    size_t size;
    ssize_t read_size;
    std::array<uint8_t,1024> read_buffer;
    while(!_quit)
    {
        FD_ZERO(&read_set);
        FD_ZERO(&write_set);
        FD_SET(STDIN_FILENO,&read_set);
        FD_SET(STDOUT_FILENO,&write_set);
        max_fd = std::max(STDIN_FILENO,STDOUT_FILENO);
        FD_SET(_socket,&read_set);
        FD_SET(_socket,&write_set);
        if(_socket > max_fd)
        {
            max_fd = _socket;
        }
        if(select(max_fd + 1,&read_set,&write_set,nullptr,nullptr) < 0)
        {
            FD_ZERO(&read_set);
            FD_ZERO(&write_set);
        }
        if(FD_ISSET(STDIN_FILENO,&read_set))
        {
            std::getline(std::cin,line_buffer);
            shared_header = std::make_shared<netpong::message_header>();
            shared_buffer = std::make_shared<std::vector<uint8_t>>();
            type_string = line_buffer.substr(0,4);
            if(type_string == "PING")
            {
                type = netpong::message_types::PING;
            }
            else if(type_string == "PING")
            {
                type = netpong::message_types::PING;
            }
            else if(type_string == "PING")
            {
                type = netpong::message_types::PING;
            }
            else
            {
                type = -1;
            }

            size = 0;

            if(line_buffer.size() > 4)
            {
                shared_buffer->resize(line_buffer.size() - 4);
                std::memcpy(
                    shared_buffer->data(),
                    line_buffer.data() + 4,
                    shared_buffer->size());
                size = shared_buffer->size();
            }

            if(type != -1)
            {
                netpong::pack_message_header(
                    shared_header.get(),
                    type,
                    size);
                send_request(
                    shared_header,
                    shared_buffer);
            }
            else
            {
                std::cerr << "Bad request type" << std::endl;
            }
        }

        if(FD_ISSET(_socket,&read_set))
        {
            std::cerr << "readable socket" << std::endl;
            do
            {
                std::cerr << "reading socket" << std::endl;
                read_size = read_socket(
                    _socket,
                    read_buffer.data(),
                    read_buffer.size());
                if(read_size > 0)
                {
                    std::cerr << "good read(" << read_size << ")" << std::endl;
                    std::cout.write(
                        reinterpret_cast<const char* const>(
                            read_buffer.data()),
                        read_size);
                }

            } while (read_size > 0);
            if(read_size < 0)
            {
                FD_ZERO(&read_set);
            }
            std::cout << std::endl;
        }
    }
    std::cerr << "Exiting main loop" << std::endl;
}

void netpong::client::send_request(
    const std::shared_ptr<netpong::message_header>& shared_header,
    const std::shared_ptr<std::vector<uint8_t>>& shared_buffer)
{
    _threadpool.call_async([&]
    {
        std::unique_lock<std::mutex> lock(_write_lock);
        if((netpong::write_message_header(
            _socket,
            shared_header.get()) < 0) 
            || (netpong::write_message_body(
                    _socket,
                    shared_buffer->data(),
                    shared_buffer->size()) < 0))
        {
            std::cerr << "Socket write failed" << std::endl;
        }
    });   
}

netpong::server::server(const uint16_t port, size_t threads) : _threadpool(threads)
{
    _listener = open_socket(
        peer_types::SERVER,
        nullptr,
        port);
}

netpong::server::~server()
{
    if(_running)
    {
        stop();
    }
}

void netpong::server::start()
{
    if(listen(_listener,100) < 0)
    {
        throw std::runtime_error("listen failed");
    }
    _running = true;
}

void netpong::server::run()
{
    fd_set readable;
    int max_fd;
    ice::native_socket_t client;
    while(_running)
    {
        std::cerr << "loop" << std::endl;
        FD_ZERO(&readable);
        FD_SET(_listener,&readable);
        max_fd = _listener;
        for(netpong::session & session : _sessions)
        {
            FD_SET(session.socket,&readable);
            if(session.socket > max_fd)
            {
                max_fd = session.socket;
            }
        }
        if(select(max_fd + 1,&readable,nullptr,nullptr,nullptr) < 0)
        {
            std::cerr << "Select failed" << std::endl;
            FD_ZERO(&readable);
        }
        else
        {
            std::cerr << "select" << std::endl;
        }
        if(FD_ISSET(_listener,&readable))
        {
            std::cerr << "New connection" << std::endl;
            client = accept(_listener,nullptr,nullptr);
            if(client < 0)
            {
                std::cerr << "accept failed" << std::endl;
            }
            else
            {
                netpong::session session;
                session.socket = client;
                _sessions.push_back(
                    std::move(session));
            }
        }
        for(netpong::session & session : _sessions)
        {
            FD_ISSET(session.socket,&readable);
            std::shared_ptr<netpong::message_header> shared_header;
            std::shared_ptr<std::vector<uint8_t>> shared_buffer;
            int type;
            size_t size;
            
            shared_header = std::make_shared<
                netpong::message_header>();

            netpong::read_message_header(session.socket,shared_header.get());

            netpong::unpack_message_header(*shared_header.get(),&type,&size);

            if(size > 0)
            {
                shared_buffer = std::make_shared<
                    std::vector<uint8_t>>(size);
                netpong::read_message_body(
                    session.socket,
                    shared_buffer->data(),
                    shared_buffer->size());
                handle_request(
                    session,
                    shared_header,
                    shared_buffer);
            }
        }
    }
}

void netpong::server::stop()
{
    _running = false;
    while(!_sessions.empty())
    {
        close_socket(_sessions.front().socket);
        _sessions.pop_front();
    }
    _threadpool.shutdown();
    close_socket(_listener);
}


void netpong::server::handle_request(
    netpong::session & sender, 
    const std::shared_ptr<netpong::message_header>& shared_header,
    const std::shared_ptr<std::vector<uint8_t>>& shared_buffer)
{
    std::cerr << "Handling request: ";
    
    if(shared_buffer->size() > 0)
    {
        std::cerr.write(
            reinterpret_cast<const char* const>(
                shared_buffer->data()),
            shared_buffer->size());
    }

    std::cerr << std::endl;
    
    switch(shared_header->type)
    {
        case netpong::message_types::PING:
            _threadpool.call_async([&]
            {
                std::unique_lock<std::mutex>(sender.write_lock);
                if((netpong::write_message_header(
                        sender.socket,
                        shared_header.get()) < 0) 
                    ||(netpong::write_message_body(
                        sender.socket,
                        shared_buffer->data(),
                        shared_buffer->size()) < 0))
                {
                    std::cerr << "close sender" << std::endl;
                }
            });
            break;
        case netpong::message_types::PONG:
            for(netpong::session & session : _sessions)
            {
                if(session.socket != sender.socket)
                {
                    _threadpool.call_async([&]
                    {
                        std::unique_lock<std::mutex>(sender.write_lock);
                        if((netpong::write_message_header(
                                sender.socket,
                                shared_header.get()) < 0) 
                            ||(netpong::write_message_body(
                                sender.socket,
                                shared_buffer->data(),
                                shared_buffer->size()) < 0))
                        {
                            std::cerr << "close session" << std::endl;
                        }
                    });
                }
            }
            break;
        case netpong::message_types::QUIT:
            std::cerr << "close session" << std::endl; 
            close_socket(sender.socket);
            _sessions.remove(sender);
            break;
        default:
            break;
    }
}