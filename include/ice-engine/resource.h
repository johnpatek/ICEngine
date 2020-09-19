#ifndef RESOURCE_H
#define RESOURCE_H
#include "engine.h"
#include "network.h"
#include "threadpool.h"
#include <unordered_map>
#include <unordered_map>
#include <tuple>
#include "json11.h"

namespace ice
{

class resource
{
private:
    Json _resource_data;
public:
    bool has_property(
        const std::string& name) const;
    Json get_property(
        const std::string& name) const;
    void set_property(
        const std::string& name, 
        const Json& value);
};

class resource_manager
{
private:
    std::unordered_map<std::string,resource> _resource_table;
public:
    bool has(const std::string& name) const;
    void create(const std::string& name, const resource& data);
    void update(const std::string& name, const resource& data);
    void destroy(const std::string& name);
};

}
#endif