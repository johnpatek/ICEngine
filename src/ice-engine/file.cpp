#include "ice-engine/file.h"

// System calls
static int32_t system_open(const char * const path, uint16_t flags);

static int32_t system_close(int32_t fd);

static int32_t system_read(int32_t fd, uint8_t * const buf, uint32_t len);

static int32_t system_write(int32_t fd, const uint8_t * const buf, uint32_t len);



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
    return reinterpret_cast<file_handle*>(
        _file_handle.get())->error_message;
}

bool ice::file::has_error() const
{
    return reinterpret_cast<file_handle*>(
        _file_handle.get())->has_error;
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
    file_descriptor = system_open(path.c_str(),open_flags);
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

// System calls
static int32_t system_open(const char * const path, uint16_t flags)
{
    int32_t result;
#ifdef WIN32
    result = _open(path,flags);
#else
    
#endif
}

static int32_t system_close(int32_t fd);

static int32_t system_read(int32_t fd, uint8_t * const buf, uint32_t len);

static int32_t system_write(int32_t fd, const uint8_t * const buf, uint32_t len);
