#include "ice-engine/argparse.h"
#include "ice-engine/network.h"
#include <iostream>
#include <thread>
#include <array>

void server_main(
    const std::string cert_path, 
    const std::string key_path, 
    const int port);

void process_request(
    ice::ssl_context& ctx,
    const ice::native_socket_t desc);

int main(const int argc, const char ** argv)
{
    argparse::ArgumentParser parser("server","An example server app.");
    parser.add_argument("-p","--port","port",true);
    parser.add_argument("-c","--cert","cert",true);
    parser.add_argument("-k","--key","key",true);
    parser.enable_help();
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    try
    {
        auto error = parser.parse(argc, argv);
        if(!error)
        {
            server_main(
                parser.get<std::string>("cert"),
                parser.get<std::string>("key"),
                parser.get<int>("port"));
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

const uint32_t BUF_SIZE = 1024;
const std::string reply = "Message from server";

void server_main(
    const std::string cert_path, 
    const std::string key_path, 
    const int port)
{
    ice::native_socket_t srv;
    std::vector<std::thread> threads;
    ice::ssl_context ctx(
        ice::SERVER_TCP_SOCKET,
        cert_path,
        key_path);
    
    threads.push_back(std::thread([&ctx,&srv]
    {
        ice::native_socket_t con;
        con = accept(srv,nullptr,nullptr);
        while(con != -1)
        {
            process_request(ctx,con);
            con = accept(srv,nullptr,nullptr);
        }
    }));

}

void process_request(
    ice::ssl_context& ctx,
    const ice::native_socket_t desc)
{
    ice::ssl_socket sock(ctx,desc);
    sock.accept();
    
}