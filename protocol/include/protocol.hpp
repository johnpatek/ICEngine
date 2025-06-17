#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <protocol/client.hpp>

namespace protocol
{
    std::shared_ptr<client> create_grpc_client(const std::string &address, int port);
}

#endif