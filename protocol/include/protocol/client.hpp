#ifndef PROTOCOL_CLIENT_HPP
#define PROTOCOL_CLIENT_HPP

#include <nsg.hpp>

namespace protocol
{


class client
{
public:
    virtual void connect(const std::string &address, int port) = 0;
    virtual void disconnect() = 0;
    std::shared_ptr<auth_app> get_auth_app();
    std::shared_ptr<nsg_app> get_nsg_app();
protected:
    virtual std::shared_ptr<auth_app> create_auth_app() = 0;
    virtual std::shared_ptr<nsg_app> create_nsg_app() = 0;
};


}

#endif