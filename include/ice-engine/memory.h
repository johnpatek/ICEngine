#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>
#include <array>
#include <vector>

namespace ice
{
    class heap_pool
    {

    };

    template<class Type> class heap_allocator : public std::allocator<Type>
    {

    };

    template<class PoolType> class pool_allocator
    {
        
    };

    typedef std::vector<uint8_t> heap_buffer;

    template<uint32_t BufferSize> class stack_buffer : public std::array<uint8_t,BufferSize>
    {
        // TODO: add stuff if necessary
    };  
}

#endif