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
class client;

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


typedef std::function<void(engine&)> client_callback_t;

class client
{
private:
    bool _running;
    engine _engine;
    resource_manager _resource_manager;
    CLIENT_HANDLER(key_event);
    CLIENT_HANDLER(mouse_event);
    CLIENT_HANDLER(quit_event);
    CLIENT_HANDLER(window_event);
public:
    client(size_t threads);
    
    client(size_t threads, const std::string& host, uint16_t port);

    ~client();

    void create_resource(
        const std::string& name, 
        const resource& data);

    void update_resource(
        const std::string& name, 
        const resource& data);
    
    void destroy_resource(
        const std::string& name);

    void connect(std::string& host, uint16_t port);
    
    bool is_connected();
    
    void disconnect();

    void set_init_function(const server_callback_t& function);
    void set_loop_function(const server_callback_t& function);

    void init(int subsystem_flags);
    void loop();
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

typedef std::function<void(engine&)> server_callback_t;

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
    
    void create_resource(
        const std::string& name, 
        const resource& data);

    void udpate_resource(
        const std::string& name, 
        const resource& data);
    
    void destroy_resource(
        const std::string& name);

    void set_start_function(const server_callback_t& function);
    void set_loop_function(const server_callback_t& function);
    void set_stop_function(const server_callback_t& function);

    void start();
    void loop();
    void stop();
};
}