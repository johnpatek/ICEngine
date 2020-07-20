#include "echo.h"
#include <ice-engine/argparse.h>


const uint32_t BUFFER_SIZE = 1024;
const std::string SERVER_MESSAGE("Message from the server.\n");

static void server_main(
    const std::string& cert,
    const std::string& key,
    const uint16_t port,
    const uint32_t threads);

int main(const int argc, const char ** argv)
{
    ice::argument_parser parser("tcp-server","A simple tcp server example.");
    parser.add_argument("-p","--port","port",true);
    parser.add_argument("-c","--cert","cert",true);
    parser.add_argument("-k","--key","key",true);
    parser.add_argument("-t","--threads","threads",false);
    parser.enable_help();
    uint32_t threads = std::thread::hardware_concurrency();

    if(!parser.parse(argc,argv) 
        && parser.exists("port") 
        && parser.exists("key") 
        && parser.exists("cert"))
    {
        if(parser.exists("threads"))
        {
            threads = parser.get<uint32_t>("threads");
        }

        try
        {
            server_main(
                parser.get<std::string>("cert"),
                parser.get<std::string>("key"),
                parser.get<uint16_t>("port"),
                threads);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Server Fault: " << e.what() << std::endl;
        }
    }
    else
    {
        parser.print_help();
    }

    return 0;
}

#ifdef _WIN32
static bool running;
static BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType)
{
  running = false;
  return TRUE;
}
#else
void signal_handler(int signal)
{

}
#endif

static void server_main(
    const std::string& cert,
    const std::string& key,
    const uint16_t port,
    const uint32_t threads)
{
#ifdef _WIN32    
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2),&data);
#endif

    echo::server server(
        cert,
        key,
        port, 
        threads);

#ifdef _WIN32 
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
#else
    struct sigaction sigaction_handler;
    sigaction_handler.sa_handler = signal_handler;
    sigemptyset(&sigaction_handler.sa_mask);
    sigaction_handler.sa_flags = 0;
#endif    
    server.start();

#ifdef _WIN32
    running = true;
    while(running)
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(500));
    }
#else
    pause();
#endif
    
    server.stop();

#ifdef _WIN32
    WSACleanup();
#endif
}