#include "common.h"
#include "ice-engine/argparse.h"


class server
{
private:
    ice::ssl_context _ctx;
    ice::native_socket_t _srv;
    std::string _file_path;
    std::thread _thread;
    bool _running;
    // research mutex
    std::mutex _file_lock;

    void log_entry(
        ice::ssl_socket & client_socket, 
        uint32_t timestamp, 
        uint32_t length)
    {
        // research mutex
        common::response_header response_header;
        std::fstream file(_file_path, std::ios::out | std::ios::app);


    }
    
    void dump_entries(ice::ssl_socket & client_socket)
    {
        // research mutex
        common::response_header response_header;
        uint32_t file_size = common::file_size(_file_path);

        if(file_size > 0)
        {
            std::fstream file(_file_path, std::ios::in);
            response_header.status = common::status_codes::OK;
        }
        else
        {
            response_header.status = common::status_codes::ERR;
            response_header.length = 0;
        }
    }
    
    void handle_request(ice::ssl_socket & client_socket)
    {
        common::request_header request_header;

        if(client_socket.read(
            reinterpret_cast<uint8_t*>(&request_header),
            common::REQUEST_HEADER_SIZE) < 0)
        {
            throw std::runtime_error("Failed to read request header");
        }



    }

public:
    server(
        const std::string& cert_file, 
        const std::string& key_file,
        const uint16_t port) : _ctx(
            ice::SERVER_TCP_SOCKET,
            cert_file,
            key_file)
    {
        _srv = common::open_socket(common::SERVER, nullptr, port);
        _file_path = "output_file.txt";
        std::fstream file_stream(_file_path);
    }

    void start()
    {
        listen(_srv,3);
        _thread = std::thread([this]
        {
            this->run();
        });
        _running = true;
    }

    void run()
    {
        bool loop(true);
        ice::native_socket_t client_socket;

        while(loop)
        {
            client_socket = accept(_srv,nullptr,nullptr);

            if(client_socket >= 0)
            {
                ice::ssl_socket secure_socket(_ctx, client_socket);
                if(secure_socket.accept() < 0)
                {
                    throw std::runtime_error("SSL accept failed");
                }
                handle_request(secure_socket);
            }
            else // client_socket < 0 indicates error
            {
                loop = false;
            }
        }
    }

    void stop()
    {
        common::close_socket(_srv);
        _thread.join();
        _running = false;
    }

    ~server()
    {
        if(_running)
        {
            stop();
        }
        std::remove(_file_path.c_str());
    }
};


void server_main(
    const std::string& cert,
    const std::string& key,
    const uint16_t port);

int main(const int argc, const char ** argv)
{
    ice::argument_parser parser("server","A simple tcp server example.");
    parser.add_argument("-p","--port","port",true);
    parser.add_argument("-c","--cert","cert",true);
    parser.add_argument("-k","--key","key",true);
    parser.enable_help();


#ifdef _WIN32    
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2),&data);
#endif

    if(!parser.parse(argc,argv) 
        && parser.exists("port") 
        && parser.exists("key") 
        && parser.exists("cert"))
    {
        try
        {
            server_main(
                parser.get<std::string>("cert"),
                parser.get<std::string>("key"),
                parser.get<uint16_t>("port"));
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

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}

bool running;

void signal_callback_handler(int signum);

void server_main(
    const std::string& cert,
    const std::string& key,
    const uint16_t port)
{
    running = true;
    signal(SIGINT, signal_callback_handler);
    while(running)
    {
        std::this_thread::yield();
    }
}

void signal_callback_handler(int signum)
{
    if(signum == CTRL_C_EVENT)
    {
        running = false;
    }
}