#include "common.h"
#include "ice-engine/argparse.h"
#include <shared_mutex>
#include <mutex>
#include <vector>
// Server class

class server
{
private:
    ice::ssl_context _ctx;
    ice::native_socket_t _srv;
    std::string _file_path;
    std::thread _thread;
    std::vector<std::thread> thread_list;
    uint8_t threads;
    bool _running;
    // research mutex
    std::shared_timed_mutex _file_lock;


    /**
     * Assignment 3: make sure the file is locked during this 
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
        std::unique_lock<std::shared_timed_mutex> file_lock(_file_lock, std::defer_lock);
        common::response_header response_header;
        std::fstream file(_file_path, std::ios::out | std::ios::app);
        std::vector<uint8_t> buffer;

        common::pack_response_header(&response_header, 
            common::status_codes::ERR, 0);

        if(file.good() || length > 0)
        {
            response_header.status = common::status_codes::OK;
            
            buffer.resize(length);
            
            common::read_request_body(client_socket,
                buffer.data(), 
                static_cast<uint32_t>(buffer.size()));

	    file_lock.lock();
            file.write(reinterpret_cast<const char*>(buffer.data()),
                buffer.size());
            
            file << std::endl;
	    file_lock.unlock();
	 }
        
        common::write_response_header(
            client_socket,&response_header);           
    }
    
    /**
     * Assignment 4: make sure the file is locked during this 
     * operation so it can only accept read operations.
     * 
     * recommended solutions: std::shared_lock
     */
    void dump_entries(ice::ssl_socket & client_socket)
    {
        // research mutex
        std::shared_lock<std::shared_timed_mutex> file_lock(_file_lock, std::defer_lock);
        common::response_header response_header;
        uint32_t file_size = common::file_size(_file_path);
        std::array<uint8_t, common::BUFFER_SIZE> buffer;

        if(file_size > 0)
        {
            std::fstream file(_file_path, std::ios::in);
            
            common::pack_response_header(
                &response_header,
                common::status_codes::OK, 
                file_size);
            
            common::write_response_header(
                client_socket, 
                &response_header);

            while(file.good())
            {
	        file_lock.lock();

                file.read(
                    reinterpret_cast<char*>(
                        buffer.data()),
                    static_cast<uint32_t>(buffer.size()));

                common::write_response_body(client_socket, 
                    buffer.data(), static_cast<uint32_t>(file.gcount()));
                file_lock.unlock();
            }

        }
        else
        {
            error_response(client_socket, "No entries available");
        }
    }

    void error_response(ice::ssl_socket & client_socket, 
        const std::string & message)
    {
        common::response_header response_header;
        uint32_t message_size = static_cast<uint32_t>(message.size());
        
        common::pack_response_header(&response_header,
            common::status_codes::ERR, message_size);

        common::write_response_header(client_socket, &response_header);

        common::write_response_body(client_socket,
            reinterpret_cast<const uint8_t * const>(message.data()),
            message_size);
    }
    
    void handle_request(ice::ssl_socket & client_socket)
    {
        common::request_header request_header;

        common::read_request_header(client_socket,&request_header);

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
            error_response(client_socket,"Invalid request command");
        }
    }

public:
    server(
        const std::string& cert_file, 
        const std::string& key_file,
        const uint16_t port,
	uint8_t num_threads) : _ctx(
            ice::SERVER_TCP_SOCKET,
            cert_file,
            key_file)
	    
    {
        _srv = common::open_socket(common::SERVER, nullptr, port);
        _file_path = "output_file.txt";
        std::fstream file_stream(_file_path, std::ios::out);
	threads = num_threads;
        thread_list.reserve(threads);
    }

    /**
     * Assignment 2: rewrite the function so the server is accepting
     * connections on multiple threads
     * 
     * recommended solutions: add std::vector as a class member
     * and add threads to it.
     */
    void start()
    {
        listen(_srv,3);
	// iterate for num threads
        while(thread_list.size() < thread_list.capacity())
	{ 
           thread_list.push_back(std::thread([this]
           {
               this->run();
           }));
	}
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
	for (std::vector<std::thread>::iterator it = thread_list.begin();
	     it < thread_list.end(); ++it)
	{
            it->join();
	}
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


static void server_main(
    const std::string& cert,
    const std::string& key,
    const uint16_t port,
    const uint8_t threads);


/**
 * Assignment 1: Add a command line flag to specify the number of 
 * threads
 * 
 * recommended solutions: use the exists function and get 
 * function and also update the server class and server_main
 * accordingly
 */
int main(const int argc, const char ** argv)
{
    ice::argument_parser parser("server",
        "A simple tls logger example.");
    parser.add_argument("-p","--port","port",true);
    parser.add_argument("-c","--cert","cert",true);
    parser.add_argument("-k","--key","key",true);
    parser.add_argument("-t","--threads","threads",true);
    parser.enable_help();


#ifdef _WIN32    
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2),&data);
#endif

    if(!parser.parse(argc,argv) 
        && parser.exists("port") 
        && parser.exists("key") 
        && parser.exists("cert")
	&& parser.exists("threads"))
    {
        try
        {
            server_main(
                parser.get<std::string>("cert"),
                parser.get<std::string>("key"),
                parser.get<uint16_t>("port"),
		parser.get<uint8_t>("threads"));
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

static bool running;

#ifdef _WIN32
static BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType)
{
  running = false;
  return TRUE;
}
#else
void signal_handler(int signal)
{
    running = false;
}
#endif

static void server_main(
    const std::string& cert,
    const std::string& key,
    const uint16_t port,
    const uint8_t threads)
{
    running = true;
    server log_server(cert, key, port, threads);
#ifdef _WIN32 
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
#else
    struct sigaction sigaction_handler;
    sigaction_handler.sa_handler = signal_handler;
    sigemptyset(&sigaction_handler.sa_mask);
    sigaction_handler.sa_flags = 0;
#endif    
    log_server.start();

#ifdef _WIN32    
    while(running)
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(300));
    }
#else
    pause();
#endif
    log_server.stop();
}
