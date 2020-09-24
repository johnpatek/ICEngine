#include <ICEngine.h>
#include <sstream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#ifdef _WIN32
#else
#include <unistd.h>
#endif

namespace netpong
{
    class client
    {

    };

    class server
    {
    public:
        server(uint16_t port, size_t threads) {}
        void start(){}
        void run(){}
        void stop(){}
        
    };
}