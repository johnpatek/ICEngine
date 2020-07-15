#include "common.h"
#include "ice-engine/argparse.h"

class log_client
{
private:
    ice::ssl_context _ctx;
    std::string _host;
    uint16_t _port;

    void send_request(uint8_t command, const uint8_t * const buf, uint32_t len)
    {
        ice::native_socket_t cli = open_socket(CLIENT, _host.c_str(), _port);
        ice::ssl_socket sock(_ctx, cli);
        int32_t read_size, write_size;
        std::array<uint8_t,BUFFER_SIZE> buffer;
        uint8_t status;
        bool read_loop;

        if(sock.connect() < 0)
        {
            throw std::runtime_error("SSL connection error");
        }

        write_size = sock.write(&command,1);
        
        if(write_size < 0)
        {
            throw std::runtime_error("Error: Unable to write header");
        }

        write_size = sock.write(buf,len);
        
        if(write_size < 0)
        {
            throw std::runtime_error("Error: Unable to write header");
        }

        read_size = sock.read(&status,1);

        read_loop = (read_size == 1) && (status == 0) && (command == DUMP);

        while(read_loop)
        {
            read_size = sock.read(buffer.data(),buffer.size());

            if(read_size > 0)
            {
                std::cerr.write(
                    reinterpret_cast<const char* const>(
                        buffer.data()),
                    buffer.size());
            }
            else if(read_size == 0)
            {
                read_loop = false;
            }
            else
            {
                throw std::runtime_error("SSL Socket read error");
            }
        }
        std::cerr << std::endl;
        // No need to close cli as it is closed by ~sock()
    }

public:
    log_client(const std::string& host, uint16_t port) : _ctx(ice::CLIENT_TCP_SOCKET)
    {
        _host = host;
        _port = port;
    }

    void log(const std::string& message)
    {
        if(message.size() > 0)
        {
            send_request(
                LOG,
                reinterpret_cast<const uint8_t* const>(
                    message.c_str()),
                message.size());
        }
    }

    void dump()
    {
        send_request(
            DUMP,
            nullptr,
            0);
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
    log_client client(address,port);
    std::string command, message_buffer;

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
            std::getline(std::cin,message_buffer);
            client.log(message_buffer);
        }
        else if(command == "DUMP")
        {
            client.dump();
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