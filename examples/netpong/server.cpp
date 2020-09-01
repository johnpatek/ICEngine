#include <ICEngine.h>
#include <ice-engine/argparse.h>
#include "netpong.h"

netpong::server * p_server;

#ifdef _WIN32
static bool running;
static BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType)
{
    p_server->stop();
    return TRUE;
}
#else
void signal_handler(int signal)
{
    p_server->stop();
}
#endif

void server_main(
    const uint16_t port,
    const size_t threads);

int main(int argc, const char ** argv)
{
    try
    {
        server_main(
            35000,
            std::thread::hardware_concurrency() * 2);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Server Error: " 
                    << e.what() 
                    << std::endl;
    }
    return 0;
}

void server_main(
    const uint16_t port,
    const size_t threads)
{
    netpong::server server(port,threads);
    p_server = &server;

    p_server->start();

#ifdef _WIN32 
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
#else
    struct sigaction sigaction_handler;
    sigaction_handler.sa_handler = signal_handler;
    sigemptyset(&sigaction_handler.sa_mask);
    sigaction_handler.sa_flags = 0;
    sigaction(SIGINT,&sigaction_handler,nullptr);
#endif
    p_server->run();
}
