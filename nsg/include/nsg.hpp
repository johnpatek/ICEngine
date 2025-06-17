#ifndef NSG_HPP
#define NSG_HPP

#include <nsg/command.hpp>
#include <nsg/consumer.hpp>
#include <nsg/producer.hpp>
#include <nsg/reader.hpp>
#include <nsg/writer.hpp>

namespace nsg
{
    class scheduler
    {
    public:
        class task
        {
        public:
            void complete();
        private:
            task(const std::shared_ptr<consumer> &consumer,
                 const std::shared_ptr<command_buffer> &buffer);
            const std::shared_ptr<consumer>& _consumer;
            const std::shared_ptr<command_buffer>& _buffer;
            friend class scheduler;
        };
        static std::shared_ptr<scheduler> default_scheduler;

        void dispatch_commands(const std::shared_ptr<consumer> &consumer, const std::shared_ptr<command_buffer> &command_buffer);

        virtual void schedule_task(std::shared_ptr<task> &&task) = 0;
    };

    /**
     * @brief Records commands into the command buffer using the provided record function.
     *
     * @param writer The writer to write the command buffer.
     * @param buffer The command buffer to record commands into.
     * @param record_function The function that records commands into the producer.
     * This function creates a producer with the provided command buffer, records commands
     * using the record function, and then writes the command buffer to the writer.
     */
    void record_commands(
        const std::shared_ptr<writer> &writer,
        const std::shared_ptr<command_buffer> &buffer,
        const std::function<void(std::shared_ptr<producer> &)> &record_function);

    /**
     *  @brief Submits command buffers to the consumer.
     *  @param reader The reader that reads the command buffers.
     *  @param consumer The consumer that executes the commands.
     *  This function reads all command buffers from the reader
     *  and executes them using the consumer.
     */
    void submit_commands(const std::shared_ptr<reader> &reader,
                         const std::shared_ptr<consumer> &consumer,
                         const std::shared_ptr<scheduler> &scheduler = scheduler::default_scheduler);
}

#endif