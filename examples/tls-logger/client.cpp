#include "common.h"
#include "ice-engine/argparse.h"

class client
{
private:
    ice::ssl_context _ctx;
    std::string _host;
    uint16_t _port;

    void send_request( 
        uint8_t message_command, 
        const uint8_t * const message_buffer,
        const uint32_t message_length)
    {
        common::request_header request_header;
        common::response_header response_header;
        ice::native_socket_t client_socket = common::open_socket(
            common::CLIENT,_host.c_str(),_port);
        ice::ssl_socket secure_socket(_ctx,client_socket);
        std::array<uint8_t, common::BUFFER_SIZE> buffer;
        uint32_t read_size;
        bool loop(true);
            
        common::pack_request_header(
            &request_header,
            message_command,
            common::timestamp(),
            message_length);
        
        if(secure_socket.connect() < 0)
        {
            throw std::runtime_error("SSL connection failed");
        }

        if(secure_socket.write(
            reinterpret_cast<uint8_t*>(&request_header),
            common::REQUEST_HEADER_SIZE) < 0)
        {
            throw std::runtime_error("Failed to write request header");
        }

        if(message_length > 0 && secure_socket.write(
            message_buffer, message_length) < 0)
        {
            throw std::runtime_error("Failed to write request header");
        }

        if(secure_socket.read(
            reinterpret_cast<uint8_t*>(&response_header),
            common::RESPONSE_HEADER_SIZE) < 0)
        {
            throw std::runtime_error("Failed to read response header");    
        }

        if(response_header.status == common::status_codes::OK 
            && response_header.length > 0)
        {
            while(loop)
            {
                read_size = secure_socket.read(
                    buffer.data(), buffer.size());
                if(read_size > 0)
                {
                    std::cerr.write(reinterpret_cast<const char*>(buffer.data()),read_size);
                }
                else if(read_size == 0)
                {
                    loop = false;
                }
                else
                {
                    throw std::runtime_error("Failed to read response body");
                }
            }
            std::cerr << std::endl;
        }
    }

public:
    client(const std::string & host, const uint16_t port) : _ctx(ice::CLIENT_TCP_SOCKET)
    {
        _host = host;
        _port = port;
    }

    void log_message(const std::string & message)
    {
        send_request(
            common::command_codes::LOG,
            reinterpret_cast<const uint8_t* const>(message.c_str()),
            message.size());
    }

    void dump_messages()
    {
        send_request(common::command_codes::DUMP,nullptr,0);
    }
};

void client_main(
    const std::string& address, 
    const uint16_t port);

void print_help();


int main(const int argc, const char ** argv)
{
    ice::argument_parser parser("client","A simple tcp client example.");
    parser.add_argument("-a","--address","address",true);
    parser.add_argument("-p","--port","port",true);
    parser.enable_help();

    if(!parser.parse(argc,argv) 
        && parser.exists("address")
        && parser.exists("port"))
    {
        try
        {
            client_main(
                parser.get<std::string>("address"),
                parser.get<uint16_t>("port"));
        }
        catch(const std::exception& e)
        {
            std::cerr << "Client Error: " << e.what() << std::endl;
        }            
    }
    else
    {
        parser.print_help();
    }

    return 0;
}

void client_main(
    const std::string& address, 
    const uint16_t port)
{
    bool loop(true);
    std::string command, message;
    client log_client(address,port);

#ifdef _WIN32    
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2),&data);
#endif

    while(loop)
    {
        std::cerr << ">>>";
        
        std::getline(std::cin,command);
        
        if(command == "LOG")
        {
            std::cerr << "enter message: ";
            std::getline(std::cin,message);
            log_client.log_message(message);
        }
        else if(command == "DUMP")
        {
            log_client.dump_messages();
        }
        else if(command == "QUIT")
        {
            loop = false;
        }
        else if(command == "HELP")
        {
            print_help();
        }
        else if(command.size() > 0)
        {
            std::cerr << "Unrecognized command \"" 
                      << command 
                      << "\", enter \"HELP\" for a list of commands."
                      << std::endl;
        }
    }

#ifdef _WIN32
    WSACleanup();
#endif
}

void print_help()
{
    std::cerr << "LOG  - Log a message on the server" << std::endl
              << "DUMP - Dump messages from the server" << std::endl
              << "QUIT - Quit the client application" << std::endl
              << "HELP - Print this message" << std::endl;
}