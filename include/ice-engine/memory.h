#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>
#include <array>
#include <memory>
#include <vector>

namespace ice
{
    class memory_pool
    {
    private:
        uint8_t * _buf;
        uint32_t size;
        uint32_t free_size;

        struct memory_block
        {
            memory_block * prev;
            memory_block * next;
            uint32_t size;
            bool is_free;
        };

        const uint32_t HEADER_SIZE = sizeof(memory_block);

    public:

        memory_pool();

        memory_pool(uint8_t* buf, uint32_t size);

        // no copying
        memory_pool(const memory_pool& pool) = delete;

        memory_pool(memory_pool&& pool) = default;

        ~memory_pool();

        void aquire(uint8_t* buf, uint32_t size);

        uint8_t* release();

        void malloc(uint32_t size);

        void calloc(uint32_t size);

        void realloc(void* ptr, uint32_t size);

        void free(void * ptr);

        uint32_t free_size() const;

        uint32_t largest_free_size() const;

        uint32_t size() const;
    }; 

    class pool_allocator
    {

    };

    class heap_allocator
    {

    };

    class heap_buffer : public std::vector<uint8_t>
    {

    };

    template<uint32_t size> class stack_buffer : public std::array<uint8_t,size>
    {

    };

}

#endif