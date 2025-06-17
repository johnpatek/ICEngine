#ifndef PROTOCOL_API_HPP
#define PROTOCOL_API_HPP

#include "common.hpp"

class auth_app
{
public:
    virtual std::string authenticate(const std::string &payload) = 0;
    virtual void terminate() = 0;
};

class nsg_app
{
public:
    virtual void connect(
        const std::string &client_id, 
        const std::shared_ptr<nsg::consumer> &consumer,
        const std::shared_ptr<nsg::scheduler> &scheduler = nsg::scheduler::default_scheduler) = 0;

    virtual void disconnect(
        const std::string &client_id) = 0;
};

#endif