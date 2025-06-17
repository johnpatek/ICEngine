#ifndef NSG_COMMON_HPP
#define NSG_COMMON_HPP

#include <algorithm>
#include <functional>
#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include <variant>

namespace nsg
{
    struct command;
    class command_buffer;
    class consumer;
    class producer;
    class property;
    class reader;
    class writer;
}

#endif