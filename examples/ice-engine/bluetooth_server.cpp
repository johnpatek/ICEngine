#include "ice-engine/argparse.h"
#include "ice-engine/network.h"
#ifdef _WIN32
#include <ws2bth.h>
#include <BluetoothAPIs.h>
#else
#endif

void bluetooth_server();

int main(const int argc, const char ** argv)
{
    return 0;
}

void bluetooth_server()
{
    ice::native_socket_t sock;
    SOCKADDR_BTH addr;
    sock = socket(PF_BTH,SOCK_STREAM,BTHPROTO_RFCOMM);
    
}