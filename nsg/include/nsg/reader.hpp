#ifndef NSG_READER_HPP
#define NSG_READER_HPP

#include "common.hpp"

namespace nsg
{
    /**
     * @brief The reader interface for reading command buffers.
     *
     * This interface defines a method to read command buffers.
     */
    class reader
    {
    public:
        /**
         * @brief Reads a command buffer.
         *
         * This method attempts to read a command buffer from the input source. There are no inherent 
         * requirements on the mechanism of the reader implementation, but it is expected that buffers 
         * will remain valid until the shared pointer is released.
         *
         * @return a shared pointer to the command buffer that was read. If no command buffer is available, 
         * this method wil return nullptr.
         */
        virtual std::shared_ptr<command_buffer> read() = 0;
    };
}


#endif