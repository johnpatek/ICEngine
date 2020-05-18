#include "ice-engine/network.h"
#include <openssl/ssl.h>
#include <iostream>

static void * init_ctx(int peer_type)
{
    SSL_METHOD * method;
    SSL_CTX * ctx = NULL;
    
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    if(peer_type == ice::CLIENT_SOCKET)
    {
        method = const_cast<SSL_METHOD*>(
            TLS_client_method());
    }
    else if(peer_type == ice::SERVER_SOCKET)
    {
        method = const_cast<SSL_METHOD*>(
            TLS_server_method());
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
    ice::native_soket_t desc)
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
#ifdef _WIN32
    _close(fd);
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
    ice::native_soket_t desc)
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

void ice::ssl_socket::accept()
{
    SSL_accept(static_cast<SSL*>(_ssl.get()));
}

void ice::ssl_socket::connect()
{
    SSL_connect(static_cast<SSL*>(_ssl.get()));
}