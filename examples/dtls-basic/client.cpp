#include "ice-engine/argparse.h"
#include "ice-engine/network.h"
#include <array>
#define PORT 8080

const char * const hello = "Hello from cilent";

void client_main(const int argc, const char ** argv);

int main(const int argc, const char ** argv)
{
    try
    {
        client_main(argc,argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

void client_main(const int argc, const char ** argv)
{
    ice::native_socket_t sock;
    int32_t read_size, write_size;
    struct sockaddr_in addr;
    std::array<uint8_t,1024> buf;

    sock = socket(PF_INET,SOCK_DGRAM,0);

    if(sock < 0)
    {
        throw std::runtime_error("Socket failed");
    }

    addr.sin_family = AF_INET;
    addr.sin_port = PORT;

}