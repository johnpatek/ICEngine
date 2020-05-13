#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>
#include <array>

namespace ice
{
    class memory_pool
    {

    };

    

    template<uint32_t size> class stack_buffer : public std::array<uint8_t,size>
    {

    };


}

#endif