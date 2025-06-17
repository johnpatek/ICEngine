#ifndef PROTOCOL_SPI_HPP
#define PROTOCOL_SPI_HPP

#include "common.hpp"

namespace protocol
{
    class auth_svc
    {
    public:
        // Authenticate a user with the given payload.
        virtual std::string handle_authenticate(const std::string &payload) = 0;

        // Terminate the authentication service.
        virtual void handle_terminate() = 0;
    };

    class nsg_svc
    {
    public:
        // Connect a client with the given client ID and consumer.
        virtual void handle_connect(const std::string &client_id) = 0;
        
        virtual std::shared_ptr<nsg::writer> get_writer(const std::string &client_id) = 0;

        // Disconnect a client with the given client ID.
        virtual void handle_disconnect(const std::string &client_id) = 0;
    };
}

#endif