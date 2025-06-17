#include <nsg.hpp>

nsg::scheduler::task::task(const std::shared_ptr<nsg::consumer> &consumer,
                            const std::shared_ptr<nsg::command_buffer> &buffer)
    : _consumer(consumer), _buffer(buffer)
{
}

void nsg::scheduler::task::complete()
{
    nsg::command cmd;
    while (_buffer->pop(cmd))
    {
        switch (cmd.cmd_type)
        {
            case nsg::command::op::add:
                _consumer->handle_add(cmd.node_id, cmd.parent_id, cmd.properties);
                break;
            case nsg::command::op::remove:
                _consumer->handle_remove(cmd.node_id);
                break;
            case nsg::command::op::update:
                _consumer->handle_update(cmd.node_id, cmd.properties);
                break;
            case nsg::command::op::move:
                _consumer->handle_move(cmd.node_id, cmd.parent_id);
                break;
        }
    }
}

void nsg::scheduler::dispatch_commands(const std::shared_ptr<nsg::consumer> &consumer,
                                       const std::shared_ptr<nsg::command_buffer> &command_buffer)
{
    std::shared_ptr<nsg::scheduler::task> task(new nsg::scheduler::task(consumer, command_buffer));
    schedule_task(std::move(task));
}

class sequential_scheduler : public nsg::scheduler
{
public:
    sequential_scheduler() = default;
    void schedule_task(std::shared_ptr<task> &&task) override
    {
        task->complete();
    }
};

std::shared_ptr<nsg::scheduler> nsg::scheduler::default_scheduler = std::make_shared<sequential_scheduler>();

void record_commands(
    const std::shared_ptr<nsg::writer> &writer,
    const std::shared_ptr<nsg::command_buffer> &buffer,
    const std::function<void(std::shared_ptr<nsg::producer> &)> &record_function)
{
    std::shared_ptr<nsg::producer> producer = std::make_shared<nsg::producer>(buffer);
    record_function(producer);
    writer->write(buffer);
}

void submit_commands(const std::shared_ptr<nsg::reader> &reader,
                     const std::shared_ptr<nsg::consumer> &consumer,
                     const std::shared_ptr<nsg::scheduler> &scheduler)
{
    for (std::shared_ptr<nsg::command_buffer> command_buffer = reader->read();
         command_buffer != nullptr;
         command_buffer = reader->read())
    {
        scheduler->dispatch_commands(consumer, command_buffer);
    }
}
