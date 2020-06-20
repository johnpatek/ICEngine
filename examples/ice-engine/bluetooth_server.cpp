#include "ice-engine/argparse.h"
#include "ice-engine/network.h"
#ifdef _WIN32
#include <ws2bth.h>
#include <BluetoothAPIs.h>
#else
#endif
#include <array>
#include <atomic>

const uint32_t BUF_SIZE = 1024;
const std::string SRV_MSG("Hello from the server");
std::atomic<bool> run;

// Handlers for ctrl-c shutdown
#ifdef _WIN32
BOOL WINAPI signal_handler(DWORD signal) 
{
    if (signal == CTRL_C_EVENT)
    {
        run.store(false);
    }
    return TRUE;
}
#else
void signal_handler(int s)
{
    run.store(false);
}
#endif

void bluetooth_server(
    const std::string & cert_path,
    const std::string & key_path);

int main(const int argc, const char ** argv)
{
    ice::argument_parser parser(
        "bluetooth-server", 
        "A server for processing requests over bluetooth");
    std::string cert_path;
    std::string key_path;
    parser.add_argument("-c","--cert","cert",true);
    parser.add_argument("-k","--key","key",true);
    parser.enable_help();
    auto error = parser.parse(
        argc,
        argv);
    try
    {
        if(!error && parser.exists("cert") && parser.exists("key"))
        {
            run.store(true);
            bluetooth_server(
                parser.get<std::string>("cert"),
                parser.get<std::string>("key"));
        }
        else
        {
            parser.print_help();
        }
    }
    catch(const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

void bluetooth_server(
    const std::string & cert_path,
    const std::string & key_path)
{
    ice::ssl_context ctx(
        ice::SERVER_TCP_SOCKET,
        cert_path,
        key_path);
    ice::native_socket_t sock, cli;
    bool loop(true);
    std::array<uint8_t,BUF_SIZE> buf;
    int32_t read_size, write_size;
#ifdef _WIN322
    SOCKADDR_BTH addr;
    sock = socket(PF_BTH,SOCK_STREAM,BTHPROTO_RFCOMM);
#else
    // implement bluetooth from Unix API
#endif
    if(sock < 0)
    {
        throw std::runtime_error("Socket failed.");
    }

    if(listen(sock,3) < 0)
    {
        throw std::runtime_error("Listen failed.");        
    }

    while(loop && run.load())
    {
        cli = accept(sock,nullptr,nullptr);
        if(cli != -1)
        {
            ice::ssl_socket secure_sock(
                ctx,
                cli);
            secure_sock.accept();
            read_size = secure_sock.read(buf.data(),buf.size());
            if(read_size < 0)
            {
                throw std::runtime_error("Bad read");
            }
            std::cerr << "Message Received:" 
                      << std:: endl
                      << "Address: ";
            std::cerr.write(
                reinterpret_cast<const char*>(
                    buf.data()),
                buf.size());
            std::cerr << std::endl;
            write_size = secure_sock.write(
                reinterpret_cast<const uint8_t * const>(
                    SRV_MSG.data()),
                SRV_MSG.size());
            if(write_size < 0)
            {
                
            }
        }
        else
        {
            loop = false;
        }
    }
}