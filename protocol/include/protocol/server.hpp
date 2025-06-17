#ifndef PROTOCOL_SERVER_HPP
#define PROTOCOL_SERVER_HPP

#include "common.hpp"

namespace protocol
{
    class server
    {
    public:
        virtual ~server() = default;

        // Start the server with the given address and port.
        virtual void start(const std::string &address, int port) = 0;

        // Stop the server.
        virtual void stop() = 0;

        virtual void register_auth_service(
            const std::shared_ptr<auth_svc> &auth_service) = 0;

        virtual void register_nsg_service(
            const std::shared_ptr<nsg_svc> &nsg_service) = 0;
    };
}

#endif