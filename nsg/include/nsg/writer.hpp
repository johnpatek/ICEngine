#ifndef NSG_WRITER_HPP
#define NSG_WRITER_HPP

#include "common.hpp"

namespace nsg
{
    /**
     * @brief The writer interface for writing command buffers.
     *
     * This interface defines a method to write command buffers.
     */
    class writer
    {
    public:
        /**
         * @brief Writes a command buffer.
         *
         * This method writes the provided command buffer to the output destination. There are no inherent
         * requirements regarding batching or synchronization, but it is recomended that the writer flushes
         * command buffers in the order they were written.
         *
         * @param buffer A shared pointer to the command buffer that will be written.
         */
        virtual void write(const std::shared_ptr<command_buffer> &buffer) = 0;
    };
}


#endif