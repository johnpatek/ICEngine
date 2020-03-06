#include <unistd.h>
#include <core/http.h>

void signal_handler(int signum){}

class http_server : public ice::core::http_server
{
public:
    // this is required to inherit constructors
    using ice::core::http_server::http_server;
    
    ice::core::string_type handle_get(
        const ice::core::string_type & path,
        const ice::core::http_headers & headers)
    {
        return ice::core::string_type(
            "GET " + path + " ok");
    }
    
    ice::core::string_type handle_post(
        const ice::core::string_type & path,
        const ice::core::http_headers & headers,
        const ice::core::byte_type * data,
        const ice::core::size_type size)
    {
        return ice::core::string_type(
            "POST " + path + " ok: " + std::string(
                (char*)data,
                size));
    }
};

int main(const int argc, const char ** argv) 
{
    std::shared_ptr<ice::core::engine> engine;
    std::shared_ptr<http_server> server;

    engine = std::make_shared<ice::core::engine>(4);
    server = std::make_shared<http_server>(
        engine,
        3000,
        4);

    signal(
        SIGINT,
        signal_handler);

    server->start();
    pause();
    server->stop();
    return 0;
}
