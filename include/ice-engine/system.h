#ifndef SYSTEM_H
#define SYSTEM_H
#include "common.h"
#if defined(_WIN32)
#else /* Unix */
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#endif

#ifdef _WIN32
#define OPEN_FLAG(FLAG) _O_ ## FLAG
#else
#define OPEN_FLAG(FLAG) O_ ## FLAG
#endif
namespace ice
{
    typedef int native_file_t;
    const native_file_t INVALID_FILE_HANDLE = -1;
#ifdef _WIN32
    typedef SOCKET native_socket_t;
    const native_socket_t INVALID_SOCKET_HANDLE = INVALID_SOCKET;
    const native_file_t STANDARD_INPUT = 0;
    const native_file_t STANDARD_OUTPUT = 1;
    const native_file_t STANDARD_ERROR = 2;
#else
    typedef int native_socket_t;
    const native_socket_t INVALID_SOCKET_HANDLE = -1;
    const native_file_t STANDARD_INPUT = STDIN_FILENO;
    const native_file_t STANDARD_OUTPUT = STDOUT_FILENO;
    const native_file_t STANDARD_ERROR = STDERR_FILENO;
#endif    

    native_socket_t open_socket(int domain, int type, int protocol);
    
    bool close_socket(native_socket_t handle);

    ssize_t read_socket(native_socket_t handle, void * const data, 
        size_t size);

    ssize_t write_socket(native_socket_t handle, 
        const void * const data, size_t size);

    enum open_modes
    {
        APPEND = OPEN_FLAG(APPEND),
        CREATE = OPEN_FLAG(CREAT),
        READ_ONLY = OPEN_FLAG(RDONLY),
        WRITE_ONLY = OPEN_FLAG(WRONLY),
        READ_WRITE = OPEN_FLAG(RDWR)
    };

    native_file_t open_file(const std::string& path, int open_mode);
    
    bool close_file(native_file_t handle);

    ssize_t read_file(native_file_t handle, void * const data, 
        size_t size);

    ssize_t write_file(native_file_t handle, const void * const data,
        size_t size);
}

#endif