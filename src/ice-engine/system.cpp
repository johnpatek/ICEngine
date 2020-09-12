#include "ice-engine/system.h"

#ifdef _WIN32
#define CLOSE_SOCKET closesocket
#define CLOSE_FILE _close
#define READ _read
#define WRITE _write
#else
#define CLOSE_SOCKET close
#define CLOSE_FILE close
#define READ read
#define WRITE write
#endif

ice::native_socket_t ice::open_socket(int domain, int type, int protocol)
{
    return socket(domain,type,protocol);
}

bool ice::close_socket(ice::native_socket_t handle)
{
    return CLOSE_SOCKET(handle) == 0;
}

ssize_t ice::read_socket(ice::native_socket_t handle, void * const data, 
    size_t size)
{
    return READ(
        static_cast<int>(handle),
        data,
#ifdef _WIN32
        static_cast<unsigned int>(size));
#else    
        size);
#endif
}

ssize_t ice::write_socket(native_socket_t handle, 
    const void * const data, size_t size)
{
    return WRITE(
        static_cast<int>(handle),
        data,
#ifdef _WIN32
        static_cast<unsigned int>(size));
#else    
        size);
#endif
}

ice::native_file_t ice::open_file(const std::string& path, int open_mode)
{
    ice::native_file_t result;
#ifdef _WIN32
     if(!_sopen_s(
        &result,
        path.c_str(),
        open_mode,
        _SH_DENYNO,
        _S_IREAD | _S_IWRITE))
    {
        result = ice::INVALID_FILE_HANDLE;
    }
#else
    result = open(
        path.c_str(),
        open_mode);
#endif
    return result;
}

bool ice::close_file(ice::native_file_t handle)
{
    return CLOSE_FILE(handle) < 0;
}

ssize_t ice::read_file(ice::native_file_t handle, void * const data, 
    size_t size)
{
    return READ(
        handle,
        data,
#ifdef _WIN32
        static_cast<unsigned int>(size));
#else    
        size);
#endif
}

ssize_t ice::write_file(ice::native_file_t handle, const void * const data,
    size_t size)
{
    return WRITE(
        handle,
        data,
#ifdef _WIN32
        static_cast<unsigned int>(size));
#else    
        size);
#endif
}