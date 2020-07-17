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


    /**
     * Assignment 2: make sure the file is locked during this 
     * operation so only one thread can access it.
     * 
     * recommended solutions: std::unique_lock
     */
    void log_entry(
        ice::ssl_socket & client_socket, 
        uint32_t timestamp, 
        uint32_t length)
    {
        // research mutex
        common::response_header response_header;
        std::fstream file(_file_path, std::ios::out | std::ios::app);
        std::vector<uint8_t> buffer;

        common::pack_response_header(&response_header, 
            common::status_codes::ERR, 0);

        if(file.good() || length > 0)
        {
            response_header.status = common::status_codes::OK;
            buffer.resize(length);
            if(client_socket.read(buffer.data(), static_cast<uint32_t>(buffer.size())) < 0)
            {
                throw std::runtime_error("Failed to read request body");
            }
            file.write(reinterpret_cast<const char*>(buffer.data()),
                buffer.size());
            file << std::endl;
        }
        
        if(client_socket.write(reinterpret_cast<const uint8_t * const>(
            &response_header),common::RESPONSE_HEADER_SIZE) < 0)
        {
            throw std::runtime_error("Failed to write response header");
        }            
    }
    

    /**
     * Assignment 2: make sure the file is locked during this 
     * operation so only one thread can access it.
     * 
     * recommended solutions: std::unique_lock
     */
    void dump_entries(ice::ssl_socket & client_socket)
    {
        // research mutex
        common::response_header response_header;
        uint32_t file_size = common::file_size(_file_path);
        std::array<uint8_t, common::BUFFER_SIZE> buffer;

        if(file_size > 0)
        {
            std::fstream file(_file_path, std::ios::in);
            response_header.status = common::status_codes::OK;
            response_header.length = common::file_size(_file_path);
            
            if(client_socket.write(reinterpret_cast<const uint8_t * const>(
                &response_header),common::RESPONSE_HEADER_SIZE) < 0)
            {
                throw std::runtime_error("Failed to write response header");
            }   
            
            while(file.good())
            {
                file.read(
                    reinterpret_cast<char*>(
                        buffer.data()),
                    buffer.size());

                if(client_socket.write(buffer.data(), file.gcount()) < 0)
                {
                    throw std::runtime_error("Failed to write response body");
                }
            }
        }
        else
        {
            response_header.status = common::status_codes::ERR;
            response_header.length = 0;
            if(client_socket.write(reinterpret_cast<const uint8_t * const>(
                &response_header),common::RESPONSE_HEADER_SIZE) < 0)
            {
                throw std::runtime_error("Failed to write response header");
            }   
            
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

        if(request_header.command == common::command_codes::LOG)
        {
            log_entry(client_socket, request_header.timestamp, 
                request_header.length);
        }
        else if(request_header.command == common::command_codes::DUMP)
        {
            dump_entries(client_socket);
        }
        else
        {
            // Should be unreachable
            // Figure out how to write an error packet
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
        std::fstream file_stream(_file_path, std::ios::out);
    }

    /**
     * Assignment 3: rewrite the function so the server is accepting
     * connections on multiple threads
     * 
     * recommended solutions: add std::vector as a class member
     * and add threads to it.
     */
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
    server log_server(cert, key, port);
    
    signal(SIGINT, signal_callback_handler);
    
    log_server.start();
    
    while(running)
    {
        std::this_thread::yield();
    }
    
    log_server.stop();
}

void signal_callback_handler(int signum)
{
    if(signum == CTRL_C_EVENT)
    {
        running = false;
    }
}