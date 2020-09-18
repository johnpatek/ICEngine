#include "ice-engine/common.h"
#include "ice-engine/engine.h"
#include "ice-engine/graphics.h"
#include "ice-engine/memory.h"
#include "ice-engine/network.h"
#include "ice-engine/resource.h"
#include "ice-engine/threadpool.h"
#include "ice-engine/system.h"
#include <unordered_map>
#include <tuple>


namespace ice
{
#define CLIENT_HANDLER(EVENT_TYPE)                                   \
public:                                                              \
    void set_ ## EVENT_TYPE ## _callback(                            \
        const EVENT_TYPE ## _callback_t& callback)                   \
    {                                                                \
        _ ## EVENT_TYPE ## _callback = callback;                     \
    }                                                                \
private:                                                             \
    std::pair<bool,EVENT_TYPE> _ ## EVENT_TYPE ## _listener;         \
    EVENT_TYPE ## _callback_t _ ## EVENT_TYPE ## _callback

class client;

typedef std::function<void(client&)> client_callback_t;

class client
{
private:
    bool _running;
    engine _engine;
    resource_client _resource_client;
    CLIENT_HANDLER(key_event);
    CLIENT_HANDLER(mouse_event);
    CLIENT_HANDLER(quit_event);
    CLIENT_HANDLER(window_event);
public:
    client(size_t threads);
    
    client(size_t threads, std::string& host, uint16_t port);

    ~client();

    void connect(std::string& host, uint16_t port);
    
    bool is_connected();
    
    void disconnect();

    void loop();
    
    void add_resource_type(
        const std::string& type_name,
        const resource_create_t& constructor,
        const resource_destroy_t& destructor,
        const resource_update_t& updater);

    bool has_resource_type(
        const std::string& type_name) const;
    
    void remove_resource_type(
        const std::string& type_name);

    void create_resource_data( 
        const std::string& resource_name,
        const std::string& type_name,
        const uint8_t * const data,
        size_t length);
    
    bool has_resource_data(
        const std::string& resource_name) const;

    void update_resource_data(
        const std::string& resource_name,
        const uint8_t * const data,
        size_t length);

    void delete_resource_data(
        const std::string& resource_name);
};

class session
{
private:
    native_socket_t _socket;
    bool _open;
    std::mutex _read_mutex,_write_mutex;
    std::list<session> _client_sessions;
public:
    session();
    session(native_socket_t socket);
    ~session();

    void open(native_socket_t socket);
    void close();

    ssize_t read(
        void * const data, 
        size_t length);

    ssize_t write(
        const void * const data, 
        size_t length);
    
    void lock_read();
    void lock_write();
    
    void unlock_read();
    void unlock_write();

    native_socket_t get_socket() const;

    session operator=(const session& rhs);
    bool operator==(const session& rhs);
};

class server;

typedef std::function<void(server&)> server_callback_t;

class server
{
private:
    native_socket_t _listener;
    bool _running;
    engine _engine;
    std::list<session> _sessions;
public:
    server(uint16_t port);
    ~server();
    
    void set_start_function(const server_callback_t& function);
    void set_loop_function(const server_callback_t& function);
    void set_stop_function(const server_callback_t& function);
    
    void start();
    void loop();
    void stop();
};
}