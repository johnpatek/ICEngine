#ifndef RESOURCE_H
#define RESOURCE_H
#include "engine.h"
#include "network.h"
#include "threadpool.h"
#include <unordered_map>
#include <unordered_map>
#include <tuple>

namespace ice
{

typedef std::function<void(uint8_t*,size_t)> resource_create_t;
typedef std::function<void(uint8_t*,size_t)> resource_destroy_t;
typedef std::function<void(uint8_t*,size_t)> resource_update_t;

class resource_client
{
private:
    ice::native_socket_t _connection;

    bool _has_connection;

    std::unordered_map<std::string,std::tuple<
        resource_create_t,
        resource_destroy_t,
        resource_destroy_t>> _resource_types;
    
    std::shared_timed_mutex _resource_types_mutex;

    std::unordered_map<
        std::string,
        std::pair<
            std::string,
            std::vector<uint8_t>>> _resource_table;

    void publish_create(
        const std::string& resource_name,
        const std::string& type_name,
        const uint8_t * const data,
        size_t length);

    void publish_update(
        const std::string& resource_name,
        const uint8_t * const data,
        size_t length);

    void publish_delete(
        const std::string& resource_name);

public:
    void add_type(
        const std::string& type_name,
        const resource_create_t& constructor,
        const resource_destroy_t& destructor,
        const resource_update_t& updater);

    bool has_type(
        const std::string& type_name) const;
    
    void remove_type(
        const std::string& type_name);

    void create_resource(
        const std::string& resource_name,
        const std::string& type_name,
        const uint8_t * const data,
        size_t length);
    
    bool has_resource(
        const std::string& resource_name) const;

    void update_resource(
        const std::string& resource_name,
        const uint8_t * const data,
        size_t length);

    void delete_resource(
        const std::string& resource_name);
};

class resource_server
{
    std::unordered_map<
        std::string,
        std::pair<
            std::string,
            std::vector<uint8_t>>> _resource_table;

    std::shared_timed_mutex _resource_table_mutex;
    

public:
    void add_client(native_socket_t client);

    void remove_client(native_socket_t client);
};

}

#endif