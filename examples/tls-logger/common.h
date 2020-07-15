#include "ice-engine/network.h"
#include <array>
#include <fstream>
#include <thread>
#include <signal.h>

const uint32_t BUFFER_SIZE = 1024;

enum commands
{
    LOG, DUMP
};

enum peer_types
{
    CLIENT, SERVER
};

ice::native_socket_t open_socket(
    int peer_type, 
    const char * const host, 
    uint16_t port);

void close_socket(ice::native_socket_t sock);