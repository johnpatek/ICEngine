#ifndef RESOURCE_H
#define RESOURCE_H
#include <unordered_map>

namespace ice
{

class resource
{
private:

public:
    bool has_property(const std::string& name) const;
    void set_property(const std::string& name, const std::string value);
    std::string get_property(const std::string& name) const;
    
}

}

#endif