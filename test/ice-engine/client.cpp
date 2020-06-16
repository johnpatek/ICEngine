#include "ice-engine/argparse.h"
#include "ice-engine/network.h"
#include <array>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <ctime>
#include <ctime>

const uint32_t BUF_SIZE = 1024;

void randomize(uint8_t * const buf, const uint32_t len);

int main(const int argc, const char ** argv)
{
    ice::argument_parser parser("client","Test client");
    parser.add_argument("-a","--address","address",true);
    parser.add_argument("-p","--port","port",true);
    parser.add_argument("-c","--count","count",true);
    parser.add_argument("-t","--threads","threads",true);
    struct sockaddr_in addr;
    uint32_t count,threads;
    std::vector<std::thread> thread_vector;

    std::atomic<uint32_t> score;

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    
    try
    {           
        parser.parse(argc,argv);
        addr.sin_family = AF_INET;
        inet_pton(
            AF_INET,
            parser.get<std::string>("address").c_str(),
            &addr.sin_addr.s_addr);
        addr.sin_port = htons(parser.get<int>("port"));

        count = parser.get<uint32_t>("count");

        threads = parser.get<uint32_t>("threads");

        ice::ssl_context secure_context(ice::CLIENT_TCP_SOCKET);

        auto start = std::chrono::steady_clock::now();

        while(thread_vector.size() < threads)
        {
            thread_vector.push_back(std::thread([&score,&secure_context,&count,&addr]
            {
                ice::native_socket_t sock;
                std::array<uint8_t,BUF_SIZE> in_buf;
                std::array<uint8_t,BUF_SIZE> out_buf;
                while(score.load() < count)
                {
                    randomize(in_buf.data(),static_cast<uint32_t>(in_buf.size()));

                    sock = socket(PF_INET,SOCK_STREAM,0);

                    if(sock < 0)
                    {
                        throw std::runtime_error("Bad socket");
                    }

                    if(connect(sock,reinterpret_cast<struct sockaddr*>(&addr),sizeof(addr)) < 0)
                    {
                        throw std::runtime_error("Bad connection");
                    }

                    ice::ssl_socket secure_socket(secure_context,sock);

                    if(secure_socket.connect() < 0)
                    {
                        throw std::runtime_error("Bad SSL connection");
                    }

                    if(secure_socket.write(in_buf.data(),static_cast<uint32_t>(in_buf.size())) < 0)
                    {
                        throw std::runtime_error("Bad write");    
                    }

                    if(secure_socket.read(out_buf.data(),static_cast<uint32_t>(out_buf.size())) < 0)
                    {
                        throw std::runtime_error("Bad read");
                    }

                    if(std::memcmp(in_buf.data(),out_buf.data(),BUF_SIZE) != 0)
                    {
                        throw std::runtime_error("Data mismatch");    
                    }

                    score++;
                }
            }));
        }

        do
        {
            std::this_thread::yield();
        }
        while(score.load() < count);

        auto end = std::chrono::steady_clock::now();

        std::cerr << score 
                  << "/" 
                  << count 
                  << " in " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
                  << "ms" 
                  << std::endl;

        for(auto & thread : thread_vector)
        {
            thread.join();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }



    for(auto & thread : thread_vector)
    {
        thread.join();
    }

#ifdef _WIN32
    WSACleanup();   
#endif

    return 0;
}


void randomize(uint8_t * const buf, const uint32_t len)
{
    for(uint32_t off = 0; off < len; off++)
    {
        *(buf + off) = static_cast<uint8_t>(rand() % 256);
    }
}