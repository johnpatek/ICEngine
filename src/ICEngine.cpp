#include "ICEngine.h"


static void add_fd(int fd, fd_set& set, int & max);

void ice::client::loop()
{
    while(_running)
    {
        ice::poll_system_events(
            _key_event_listener,
            _mouse_event_listener,
            _quit_event_listener,
            _window_event_listener);

        if(_key_event_listener.first)
        {
            _engine.call([&]
            {
                _key_event_callback(
                    _engine,
                    _key_event_listener.second);
            });
        }

        if(_mouse_event_listener.first)
        {
            _engine.call([&]
            {
                _mouse_event_callback(
                    _engine,
                    _mouse_event_listener.second);
            });
        }
        
        if(_quit_event_listener.first)
        {
            _engine.call([&]
            {
                _quit_event_callback(
                    _engine,
                    _quit_event_listener.second);
            });
        }
        
        if(_window_event_listener.first)
        {
            _engine.call([&]
            {
                _window_event_callback(
                    _engine,
                    _window_event_listener.second);
            });
        }
    }
}

void ice::server::loop()
{
    ice::native_socket_t incoming;
    fd_set read_set;
    int max_fd;
    while(_running)
    {
        max_fd = 0;
        FD_ZERO(&read_set);
        add_fd(_listener,read_set,max_fd);
        for(ice::session& session : _sessions)
        {
            add_fd(session.get_socket(),read_set,max_fd);
        }

        if(select(max_fd + 1,&read_set,nullptr,nullptr,nullptr) < 0)
        {
            _running = false;
        }

        if(FD_ISSET(_listener,&read_set))
        {
            incoming = accept(_listener,nullptr,nullptr);
        }

        for(ice::session& session : _sessions)
        {
            if(FD_ISSET(session.get_socket(),&read_set))
            {
                
            }
        }
    }
}

static void add_fd(int fd, fd_set& set, int& max)
{
    FD_SET(fd,&set);
    if(fd > max)
    {
        max = fd;
    }
}