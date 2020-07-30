#include "ice-engine/file.h"
#define HAS_FLAG(VAL, FLAG) ((VAL & FLAG) > 0)
#ifdef _WIN32
#define OPEN_FLAG(FLAG) _O_ ## FLAG
#else
#define OPEN_FLAG(FLAG) O_ ## FLAG
#endif

// System calls
static int32_t system_open(const char * const path, uint16_t flags);

static int32_t system_close(int32_t fd);

static int32_t system_read(int32_t fd, uint8_t * const buf, uint32_t len);

static int32_t system_write(int32_t fd, const uint8_t * const buf, uint32_t len);

struct native_file_handle
{
    int32_t file_descriptor;
    bool open;
    bool readable;
    bool writable;
    bool has_error;
    std::string error_message;
};

static uint16_t translate_flags(uint8_t flags);

static void * open_file(const std::string& path, const uint8_t flags);

static int close_file(void * handle);

static void file_deleter(void * handle);

ice::file::file(const std::string& path, const uint8_t flags)
{
    if(!open(path,flags))
    {
        // TODO: add error handling
    }
}

// Open/Close
bool ice::file::open(const std::string& path, const uint8_t flags)
{
    bool result;
    void * data;

    data = open_file(path,flags);

    result = (data != nullptr);

    if(result)
    {
        _file_handle = std::shared_ptr<void>(data,file_deleter);
    }

    return result;
}

bool ice::file::close()
{
    return close_file(_file_handle.get()) >= 0;
}

// IO operations
int32_t ice::file::read(uint8_t * const buf, uint32_t len)
{
    return system_read(reinterpret_cast<native_file_handle*>(
        _file_handle.get())->file_descriptor,buf,len);
}

int32_t ice::file::write(const uint8_t * const buf, uint32_t len)
{
    return system_write(reinterpret_cast<native_file_handle*>(
        _file_handle.get())->file_descriptor,buf,len);
}

// Positional operations
uint32_t ice::file::seek(uint32_t offset, int seek_position)
{
    return 0;
}

// State information
std::string ice::file::get_error() const
{
    return reinterpret_cast<native_file_handle*>(
        _file_handle.get())->error_message;
}

bool ice::file::has_error() const
{
    return reinterpret_cast<native_file_handle*>(
        _file_handle.get())->has_error;
}

bool ice::file::is_open() const
{
    return reinterpret_cast<native_file_handle*>(
        _file_handle.get())->open;
}

static uint16_t translate_flags(uint8_t flags)
{
    uint16_t result = 0;
    if(HAS_FLAG(flags,ice::APPEND))
    {
        result |= OPEN_FLAG(APPEND);        
    }

    if(HAS_FLAG(flags,ice::CREAT))
    {
        result |= OPEN_FLAG(CREAT);        
    }

    if(HAS_FLAG(flags,ice::TRUNC))
    {
        result |= OPEN_FLAG(TRUNC);        
    }

    if(HAS_FLAG(flags,ice::RDONLY))
    {
        result |= OPEN_FLAG(RDONLY);        
    }

    if(HAS_FLAG(flags,ice::WRONLY))
    {
        result |= OPEN_FLAG(WRONLY);        
    }

    if(HAS_FLAG(flags,ice::RDWR))
    {
        result |= OPEN_FLAG(RDWR);        
    }

    return result;
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
    open_flags = translate_flags(flags);
    file_descriptor = system_open(path.c_str(),open_flags);
    has_error = (file_descriptor < 0);
    open = !has_error;
    error_message = (has_error)?"Failed to open file":"";
    native_file_handle *result = reinterpret_cast<native_file_handle*>(
        malloc(sizeof(native_file_handle)));
    result->file_descriptor = file_descriptor;
    result->open = open;
    result->has_error = has_error;
    result->error_message = error_message;
    result->readable = readable;
    result->writable = writable;
    return result;    
}

static int close_file(void * handle)
{
    bool error;
    int result;
    native_file_handle * handle_ptr = reinterpret_cast<native_file_handle*>(handle);
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
    native_file_handle * handle_ptr = reinterpret_cast<native_file_handle*>(handle);
    if(handle_ptr->open)
    {
        close_file(handle);
    }
    free(handle_ptr);
}

// System calls
static int32_t system_open(const char * const path, uint16_t flags)
{
    int32_t result;
#ifdef WIN32
    result = _open(path,flags);
#else
    result = open(path,flags);    
#endif
    return result;
}

static int32_t system_close(int32_t fd)
{
    int32_t result;
#ifdef WIN32
    result = _close(fd);
#else
    result = close(fd);    
#endif
    return result;
}

static int32_t system_read(int32_t fd, uint8_t * const buf, uint32_t len)
{
    int32_t result;
#ifdef WIN32
    result = _read(fd, buf, len);
#else
    result = read(fd, buf, len);;    
#endif
    return result;
}

static int32_t system_write(int32_t fd, const uint8_t * const buf, uint32_t len)
{
    int32_t result;
#ifdef WIN32
    result = _write(fd, buf, len);
#else
    result = write(fd, buf, len);
#endif
    return result;
}