#include "ice-engine/file.h"
#ifdef _WIN32
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#else

#endif

struct file_handle
{
    int32_t file_descriptor;
    bool open;
    bool readable;
    bool writable;
    bool has_error;
    std::string error_message;
};

static void * open_file(const std::string& path, const uint8_t flags);

static int close_file(void * handle);

static void file_deleter(void * handle);

ice::file::file(const std::string& path, const uint8_t flags)
{

}

ice::file::~file()
{
    
}

// Open/Close
bool ice::file::open(const std::string& path, const uint8_t flags)
{
    return false;
}

bool ice::file::close()
{
    return false;
}

// IO operations
int32_t ice::file::read(uint8_t * const buf, uint32_t len)
{
    return 0;
}

int32_t ice::file::write(const uint8_t * const buf, uint32_t len)
{
    return 0;
}

// Positional operations
uint32_t ice::file::seek(uint32_t offset, int seek_position)
{
    return 0;
}

// State information
std::string ice::file::get_error() const
{
    return "";
}

bool ice::file::has_error() const
{
    return false;
}

bool ice::file::is_open() const
{
    return false;
}

static void * open_file(const std::string& path, const uint8_t flags)
{
    uint16_t open_flags;
    int32_t file_descriptor;
    bool open;
    bool readable;
    bool writable;
    bool has_error;
    std::string error_message;
#ifdef _WIN32
    file_descriptor = _open(path.c_str(),open_flags);
#else
    file_descriptor = open(path.c_str(),open_flags);
#endif
    has_error = (file_descriptor < 0);
    open = !has_error;
    error_message = (has_error)?"Failed to open file":"";
    
}

static int close_file(void * handle)
{
    bool error;
    int result;
    file_handle * handle_ptr = reinterpret_cast<file_handle*>(handle);
    error = (handle_ptr == nullptr) 
        || !(handle_ptr->open);
    if(!error)
    {
#ifdef _WIN32    
        result = _close(handle_ptr->file_descriptor);
#else    
        result = close(handle_ptr->file_descriptor);
#endif
        handle_ptr->open = false;
    }
    else
    {
        result = -1;
    }
    return -1;
}

static void file_deleter(void * handle)
{

}