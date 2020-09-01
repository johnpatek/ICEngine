#include <ICEngine.h>
#include <sstream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>


namespace netpong
{

enum message_types
{
    PING,  // Echo packet back to client
    PONG,  // Send packet to other clients
    QUIT   // Terminate connection
};

struct message_header
{
    uint8_t type;
    uint8_t pad1,pad2,pad3;
    uint32_t length;
};

const size_t MESSAGE_HEADER_SIZE = sizeof(message_header);

void pack_message_header(
    message_header * const header, 
    int type, 
    size_t size);

void unpack_message_header(
    const message_header& header, 
    int * const type, 
    size_t * const size);

ssize_t read_message_header(
    ice::native_socket_t socket, 
    message_header * const header);

ssize_t write_message_header(
    ice::native_socket_t socket, 
    const message_header * const header);

ssize_t read_message_body(
    ice::native_socket_t socket,
    void * const data,
    const size_t size);

ssize_t write_message_body(
    ice::native_socket_t socket,
    const void * const data,
    const size_t size);

class client
{
private:
    ice::threadpool _threadpool;
    ice::native_socket_t _socket;
    std::mutex _write_lock;
    bool _quit;
    void send_request(
        const std::shared_ptr<message_header>& shared_header,
        const std::shared_ptr<std::vector<uint8_t>>& shared_buffer);
public:
    client(const std::string& host,const uint16_t port);
    
    void main_loop();
};

struct session
{

    ice::native_socket_t socket;
    std::mutex write_lock;

    session() = default;

    session(const session& copy)
    {
        socket = copy.socket;
    }

    session operator=(const session& copy)
    {
        session result;
        result.socket = copy.socket;
        return result;
    }

    bool operator==(const session& rhs)
    {
        return socket == rhs.socket;
    }

    bool operator<(const session& rhs)
    {
        return socket < rhs.socket;
    }

    bool operator>(const session& rhs)
    {
        return socket > rhs.socket;
    }
};

class server
{
private:
    ice::native_socket_t _listener;
    bool _running;
    ice::threadpool _threadpool;
    std::list<session> _sessions;

    void handle_request(
        session & sender, 
        const std::shared_ptr<message_header>& shared_header,
        const std::shared_ptr<std::vector<uint8_t>>& shared_buffer);

public:

    server(const uint16_t port, size_t threads);
    
    ~server();
    
    void start();
    
    void run();
    
    void stop();
};

}