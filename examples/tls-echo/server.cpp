#include "echo.h"
#include <ice-engine/argparse.h>


bool * g_loop;

const size_t BUFFER_SIZE = 1024;
const std::string SERVER_MESSAGE("Message from the server.\n");

static void server_main(
    const uint16_t port,
    const size_t threads);

int main(const int argc, const char ** argv)
{
    ice::argument_parser parser("tcp-server","A simple tcp server example.");
    parser.add_argument("-p","--port","port",true);
    parser.add_argument("-c","--cert","cert",true);
    parser.add_argument("-k","--key","key",true);
    parser.add_argument("-t","--threads","threads",false);
    parser.enable_help();
    size_t threads = std::thread::hardware_concurrency();
    bool loop = true;
    g_loop = &loop;
    if(!parser.parse(argc,argv) 
        && parser.exists("port") 
        && parser.exists("key") 
        && parser.exists("cert"))
    {
        if(parser.exists("threads"))
        {
            threads = parser.get<size_t>("threads");
        }

        try
        {
            server_main(
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
    *g_loop = false;
    return TRUE;
}
#else
void signal_handler(int signal)
{
    *g_loop = false;
}
#endif



static void server_callback(echo::server * const server,ice::native_socket_t socket)
{
    echo::request_header request_header;
    echo::request_header response_header;
    size_t buffer_size;
    std::vector<uint8_t> buffer;
    int command, status;
    
    echo::read_request_header(socket,&request_header);

    echo::unpack_request_header(request_header,&command,&buffer_size);

    buffer.resize(buffer_size);

    echo::read_request_body(
        socket,
        buffer.data(),
        buffer.size());
    
    status = echo::status_codes::OK;
    switch(command)
    {
        case echo::command_codes::CAP:
            for(uint8_t & byte : buffer)
            {
                
            }
            break;
        case echo::command_codes::ECH:
            // We do not change the buffer 
            break;
        case echo::command_codes::REV:
            std::reverse(buffer.begin(),buffer.end());
            break;
        case echo::command_codes::SCR:
            std::shuffle(
                buffer.begin(),
                buffer.end(),
                server->random_engine());
            break;
        default:
            status = echo::status_codes::ERR;
    }

    buffer_size = buffer.size();

    

    server->close_connection(socket);
}

static void server_main(
    const uint16_t port,
    const size_t threads)
{
#ifdef _WIN32    
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2),&data);
#endif

    echo::server server(
        port, 
        threads,
        server_callback);

#ifdef _WIN32 
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
#else
    struct sigaction sigaction_handler;
    sigaction_handler.sa_handler = signal_handler;
    sigemptyset(&sigaction_handler.sa_mask);
    sigaction_handler.sa_flags = 0;
#endif    



#ifdef _WIN32
    WSACleanup();
#endif
}