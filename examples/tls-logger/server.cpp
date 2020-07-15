#include "common.h"
#include "ice-engine/argparse.h"

class log_server
{
private:
    // std::vector<std::thread> _threads;
    // std::mutex _mutex;
    std::thread worker_thread;
    ice::ssl_context _ctx;
    ice::native_socket_t _srv;
    bool _open;
    std::string _file_path;


    void handle_log(ice::ssl_socket & sock)
    {
        std::array<uint8_t,BUFFER_SIZE> buffer;
        uint32_t read_size;
        bool loop(true);
        std::fstream log_file(_file_path,std::ios::app | std::ios::out);
        uint8_t status = 0;

        while(loop)
        {
            read_size = sock.read(buffer.data(),buffer.size());
            if(read_size > 0)
            {
                log_file.write(
                    reinterpret_cast<const char* const>(
                        buffer.data()),
                    buffer.size());
            }
            else if(read_size == 0)
            {
                loop = false;
            }
            else
            {
                throw std::runtime_error("Bad read");
            }
        }
        sock.write(&status,1);
    }

    void handle_dump(ice::ssl_socket & sock)
    {
        std::array<uint8_t,BUFFER_SIZE> buffer;
        uint32_t read_size, write_size;
        bool loop(true);
        std::fstream log_file(_file_path,std::ios::in);

        while(loop)
        {
            log_file.read(
                reinterpret_cast<char*>(
                    buffer.data()),
                buffer.size());
            read_size = log_file.gcount();
            if(read_size > 0)
            {
                sock.write(buffer.data(),buffer.size());
            }
            else if(read_size == 0)
            {
                loop = false;
            }
            else
            {
                throw std::runtime_error("Bad read");
            }
        }
    }

    
    void handle_request(ice::ssl_socket & sock)
    {
        uint8_t command;
        if(sock.accept() < 0)
        {
            throw std::runtime_error("SSL Accept failed");
        }

        if(sock.read(&command,1) < 0)
        {
            throw std::runtime_error("Read failed");
        }

        if(command == LOG)
        {
            handle_log(sock);
        }
        else if(command == DUMP)
        {
            handle_dump(sock);
        }
        else
        {
            throw std::runtime_error("Unrecognized command");
        }
    }

public:
    log_server(
        const std::string& cert_path,
        const std::string& key_path, 
        uint16_t port) : _ctx(
            ice::SERVER_TCP_SOCKET,
            cert_path,
            key_path)
    {
        _srv = open_socket(SERVER,nullptr,port);
        _open = true;
    }

    void start()
    {
        if(listen(_srv,3) < 0)
        {
            throw std::runtime_error("Listen failed");
        }

        worker_thread = std::thread([this]
        {
            this->run();
        });
    }

    void run()
    {
        bool is_running(true);
        ice::native_socket_t cli;

        while(is_running)
        {
            cli = accept(_srv,nullptr,nullptr);
            if(cli >= 0)
            {
                ice::ssl_socket sock(_ctx,cli);
                handle_request(sock);
            }
            else
            {
                is_running = 0;
            }
        }
    }

    void stop()
    {
        close_socket(_srv);
        worker_thread.join();
    }

    ~log_server()
    {
        if(_open)
        {
            stop();
        }
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
    log_server server(cert, key, port);
    running = true;
    server.start();
    signal(SIGINT, signal_callback_handler);
    while(running)
    {
        std::this_thread::yield();
    }
    server.stop();
}

void signal_callback_handler(int signum)
{
    if(signum == CTRL_C_EVENT)
    {
        running = false;
    }
}