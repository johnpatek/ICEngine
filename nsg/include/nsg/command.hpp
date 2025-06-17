#ifndef NSG_COMMAND_HPP
#define NSG_COMMAND_HPP

#include "common.hpp"

namespace nsg
{
    class property
    {
    public:
        enum class type
        {
            boolean,
            integer,
            floating_point,
            string
        };

        property(bool value) : _value(value) {}
        property(int value) : _value(value) {}
        property(double value) : _value(value) {}
        property(const std::string &value) : _value(value) {}
        property(const char *value) : _value(std::string(value)) {}

        template <typename T>
        T get() const
        {
            return std::get<T>(_value);
        }

        template <typename T>
        void set(const T &value)
        {
            _value = value;
        }

        type get_type() const
        {
            return static_cast<type>(_value.index());
        }

    private:
        std::variant<bool, int, double, std::string> _value;
    };

    struct command
    {
        enum class op
        {
            add,
            remove,
            update,
            move
        };

        op cmd_type;
        std::string node_id;
        std::string parent_id;
        std::unordered_map<std::string, property> properties;
    };

    class command_buffer
    {
    public:
        virtual void push(const command &cmd) = 0;

        virtual bool pop(command &cmd) = 0;

        virtual bool empty() const = 0;

        virtual void clear() = 0;
    };
}

#endif