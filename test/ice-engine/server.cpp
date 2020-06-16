#include "ice-engine/argparse.h"
#include "ice-engine/network.h"
#include <iostream>
#include <thread>
#include <array>
#include <mutex>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef _WIN32
#include <unistd.h>
#endif

const uint32_t BUF_SIZE = 1024;
bool run;

#ifdef _WIN32
BOOL WINAPI signal_handler(DWORD signal) 
{
    if (signal == CTRL_C_EVENT)
    {
        run = false;
    }
    return TRUE;
}
#else
void signal_handler(int s)
{
    run = false;
}
#endif

void server_main(
    const std::string cert_path, 
    const std::string key_path, 
    const int port,
    const int threads);

int main(const int argc, const char ** argv)
{
    run = true;
    int threads = std::thread::hardware_concurrency();
    ice::argument_parser parser("server","Test server");
    parser.add_argument("-p","--port","port",true);
    parser.add_argument("-c","--cert","cert",true);
    parser.add_argument("-k","--key","key",true);
    parser.add_argument("-t","--threads","threads",false);
    parser.enable_help();
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    try
    {
        auto error = parser.parse(argc, argv);
        if(!error 
            && parser.exists("port") 
            && parser.exists("cert") 
            && parser.exists("key"))
        {
            if(parser.exists("threads"))
            {
                threads = parser.get<int>("threads");
            }

        #ifdef _WIN32
            if (SetConsoleCtrlHandler(signal_handler, TRUE)) 
            {
        #else
            struct sigaction sigIntHandler;
            sigIntHandler.sa_handler = signal_handler;
            sigemptyset(&sigIntHandler.sa_mask);
            sigIntHandler.sa_flags = 0;
            if(sigaction(SIGINT, &sigIntHandler, NULL) == 0)
            {
        #endif
                server_main(
                    parser.get<std::string>("cert"),
                    parser.get<std::string>("key"),
                    parser.get<int>("port"),
                    threads);
            }
        }
        else
        {
            parser.print_help();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

#ifdef _WIN32
    WSACleanup();   
#endif
    return 0;
}

void server_main(
    const std::string cert_path, 
    const std::string key_path, 
    const int port,
    const int threads)
{
    ice::native_socket_t srv;
    struct sockaddr_in addr;
    std::vector<std::thread> server_threads;
    ice::ssl_context ctx(
        ice::SERVER_TCP_SOCKET,
        cert_path,
        key_path);

    srv = socket(PF_INET,SOCK_STREAM,0);

    if(srv < 0)
    {
        throw std::runtime_error("Unable to create socket");
    }

    int enable = 1;
#ifdef _WIN32
    // TODO: Figure it out
#else 
    if (setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        throw std::runtime_error("setsockopt error");
    }
#endif

    if(srv < 0)
    {
        throw std::runtime_error("Unable to set socket options");
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    if(bind(srv,reinterpret_cast<struct sockaddr*>(&addr),sizeof(addr)) < 0)
    {
        throw std::runtime_error("Unable to bind socket");    
    }

    if(listen(srv,3) < 0)
    {
        throw std::runtime_error("Unable to listen");
    }

    std::cerr << "Starting on " << threads << " threads" << std::endl;

    while(server_threads.size() < threads)
    {
        server_threads.push_back(std::thread([&ctx,&srv]
        {
            std::array<uint8_t,BUF_SIZE> buf;
            int32_t read_size,write_size;
            bool loop(true);
            ice::native_socket_t con;
            while(run && loop)
            {
                con = accept(srv,nullptr,nullptr);
                if(con > 0)
                {
                    ice::ssl_socket sock(ctx,con);
                    loop = (sock.accept() == 0);
                    read_size = sock.read(buf.data(),static_cast<uint32_t>(buf.size()));
                    loop = read_size >= 0;
                    write_size = sock.write(buf.data(),read_size);
                    loop = loop && (read_size == write_size);
                    if(!loop)
                    {
                        throw std::runtime_error("server fault");
                    }
                }
                else
                {
                    loop = false;
                }
            }
        }));
    }

    while(run)
    {
        std::this_thread::yield();
    }

    std::cerr << "Joining " << threads << " threads" << std::endl;

#ifdef _WIN32
    closesocket(srv);
#else
    close(srv);
#endif

    for(auto& server_thread : server_threads)
    {
        server_thread.join();
    }
}