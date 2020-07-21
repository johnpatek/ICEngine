#include <memory>
#include <string>



namespace ice
{

enum open_flags
{
    APPEND = 1 << 1,
    CREAT = 1 << 2,
    TRUNC = 1 << 3,
    RDONLY = 1 << 4,
    RDWR = 1 << 5,
    WRONLY = 1 << 6
};

enum seek_positions
{
    SET,
    CUR,
    END,
    DATA,
    HOLE
};

class file
{
private:
    std::shared_ptr<void> _file_handle;
public:    
    file() = default;

    file(const std::string& path, const uint8_t flags);

    ~file();

    // Open/Close
    bool open(const std::string& path, const uint8_t flags);

    bool close();

    // IO operations
    int32_t read(uint8_t * const buf, uint32_t len);

    int32_t write(const uint8_t * const buf, uint32_t len);

    // Positional operations
    uint32_t seek(uint32_t offset, int seek_position);

    // State information
    std::string get_error() const;

    bool has_error() const;
    
    bool is_open() const;
};
}