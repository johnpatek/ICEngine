#include "ice-engine/network.h"
#include <openssl/ssl.h>

static void * init_ctx(int peer_type)
{
    SSL_METHOD * method;
    SSL_CTX * ctx = NULL;
    
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    if(peer_type == ice::CLIENT_TCP_SOCKET)
    {
        method = const_cast<SSL_METHOD*>(
            TLS_client_method());
    }
    else if(peer_type == ice::SERVER_TCP_SOCKET)
    {
        method = const_cast<SSL_METHOD*>(
            TLS_server_method());
    }
    else if(peer_type == ice::CLIENT_UDP_SOCKET)
    {
        method = const_cast<SSL_METHOD*>(
            DTLS_client_method());
    }
    else if(peer_type == ice::SERVER_UDP_SOCKET)
    {
        method = const_cast<SSL_METHOD*>(
            DTLS_server_method());
    }
    else
    {
        method = NULL;
    }
    
    if(method != NULL)
    {
        ctx = SSL_CTX_new(method);
    }
    return static_cast<void*>(ctx);
}

static void * init_ssl(
    const ice::ssl_context & ctx, 
    ice::native_socket_t desc)
{
    SSL * ssl;
    ssl = SSL_new(
        static_cast<SSL_CTX*>(
            ctx.data()));
    SSL_set_fd(
        ssl,
        static_cast<int>(desc));
    return static_cast<void*>(ssl);
}

static void ssl_context_deleter(void * ctx)
{
    SSL_CTX_free(static_cast<SSL_CTX*>(ctx));
}

static void ssl_deleter(void * ssl)
{
    int fd;
    fd = SSL_get_fd(static_cast<SSL*>(ssl));
    SSL_shutdown(static_cast<SSL*>(ssl));
#ifdef _WIN32
    closesocket(fd);
#else
    close(fd);
#endif
    SSL_free(static_cast<SSL*>(ssl));
}

ice::ssl_context::ssl_context(int peer_type)
{
    _ctx = std::shared_ptr<void>(
        init_ctx(peer_type),
        ssl_context_deleter);
}

ice::ssl_context::ssl_context(
    int peer_type,
    const std::string& cert_file, 
    const std::string& key_file)
{
    _ctx = std::shared_ptr<void>(
        init_ctx(peer_type),
        ssl_context_deleter);
    
    SSL_CTX_use_certificate_file(
        static_cast<SSL_CTX*>(_ctx.get()),
        cert_file.c_str(),
        SSL_FILETYPE_PEM);

    SSL_CTX_use_PrivateKey_file(
        static_cast<SSL_CTX*>(_ctx.get()),
        key_file.c_str(),
        SSL_FILETYPE_PEM);
}

void * ice::ssl_context::data() const
{
    return _ctx.get();
}

ice::ssl_socket::ssl_socket(
    const ice::ssl_context & ctx, 
    ice::native_socket_t desc)
{
    _ssl = std::shared_ptr<void>(
        init_ssl(ctx,desc),
        ssl_deleter);
}

int32_t ice::ssl_socket::read(
            uint8_t * const data, 
            uint32_t size)
{
    return SSL_read(
        static_cast<SSL*>(_ssl.get()),
        data,
        size);
}

int32_t ice::ssl_socket::write(
    const uint8_t * const data, 
    uint32_t size)
{
    return SSL_write(
        static_cast<SSL*>(_ssl.get()),
        data,
        size);
}

int32_t ice::ssl_socket::accept()
{
    return static_cast<int32_t>(SSL_accept(static_cast<SSL*>(_ssl.get())));
}

int32_t ice::ssl_socket::connect()
{
    return static_cast<int32_t>(SSL_connect(static_cast<SSL*>(_ssl.get())));
}

bool ice::ssl_socket::has_pending() const
{
    return SSL_has_pending(
        static_cast<SSL*>(_ssl.get())) > 0;
}

uint32_t ice::ssl_socket::pending() const
{
    return static_cast<uint32_t>(
        SSL_pending(
            static_cast<SSL*>(_ssl.get())));
}

ice::tls_server::tls_server(
    const std::string& cert_path,
    const std::string& key_path,
    const ice::request_handler_t& request_handler,
    const uint16_t port,
    const uint32_t threads) : _ctx(
        ice::SERVER_TCP_SOCKET,
        cert_path,
        key_path)
{
    struct sockaddr_in addr;
    _request_handler = request_handler;
    _socket = socket(PF_INET,SOCK_STREAM,0);
    addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    bind(_socket,reinterpret_cast<struct sockaddr*>(&addr),
        sizeof(sockaddr_in));
    _threads.reserve(threads);
}

ice::tls_server::~tls_server()
{
    if(_running)
    {
        this->stop();
    }
}

void ice::tls_server::start()
{
    _running = true;
    listen(_socket, 100);
    while(_threads.size() < _threads.capacity())
    {
        _threads.push_back(std::thread([this]()
        {
            this->run();
        }));
    }
}

void ice::tls_server::run()
{
    ice::native_socket_t native_socket;
    struct sockaddr_in addr;
    socklen_t addr_len;
    bool loop(true);
    while(loop)
    {
        native_socket = accept(_socket,reinterpret_cast<struct sockaddr*>(&addr),&addr_len);
        if(native_socket >= 0)
        {
            ice::ssl_socket client_socket(_ctx,native_socket);
            client_socket.accept();
            _request_handler(client_socket,reinterpret_cast<struct sockaddr*>(&addr),addr_len);
        }
        else
        {
            loop = false;
        }
    }
}

void ice::tls_server::stop()
{
#ifdef _WIN32
    closesocket(_socket);
#else
    close(_socket);
#endif
    for(std::thread & thread : _threads)
    {
        thread.join();
    }
    _running = false;
}
