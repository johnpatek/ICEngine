#include "network.h"

static SSL_CTX * init_ctx(int peer_type)
{
    SSL_METHOD * method;
    SSL_CTX * ctx = NULL;
    
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    if(peer_type == ice::CLIENT_SOCKET)
    {
        method = const_cast<SSL_METHOD*>(
            TLSv1_2_client_method());
    }
    else if(peer_type == ice::SERVER_SOCKET)
    {
        method = const_cast<SSL_METHOD*>(
            TLSv1_2_server_method());
    }
    else
    {
        method = NULL;
    }
    
    if(method != NULL)
    {
        ctx = SSL_CTX_new(method);
    }
    return ctx;
}

static void ssl_context_deleter(SSL_CTX * ctx)
{
    SSL_CTX_free(ctx);
}

static void ssl_deleter(SSL * ssl)
{
    int fd;
    fd = SSL_get_fd(ssl);
    close(fd);
    SSL_free(ssl);
}

ice::ssl_context::ssl_context(int peer_type)
{
    _ctx = std::make_shared<SSL_CTX>(
        init_ctx(peer_type),
        ssl_context_deleter);
}

ice::ssl_context::ssl_context(
    int peer_type,
    const std::string& cert_file, 
    const std::string& key_file)
{
    _ctx = std::make_shared<SSL_CTX>(
        init_ctx(peer_type),
        ssl_context_deleter);
    
    SSL_CTX_use_certificate_file(
        _ctx.get(),
        cert_file.c_str(),
        SSL_FILETYPE_PEM);

    SSL_CTX_use_PrivateKey_file(
        _ctx.get(),
        key_file.c_str(),
        SSL_FILETYPE_PEM);
}