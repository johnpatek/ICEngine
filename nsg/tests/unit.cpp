#include <nsg.hpp>
#include <maxtest.hpp>

#include <queue>

class queue_buffer : public nsg::command_buffer
{
public:
    queue_buffer() = default;

    void push(const nsg::command &cmd) override
    {
        _queue.push(cmd);
    }

    bool pop(nsg::command &cmd) override
    {
        if (_queue.empty())
            return false;

        cmd = _queue.front();
        _queue.pop();
        return true;
    }

    bool empty() const override
    {
        return _queue.empty();
    }

    void clear() override
    {
        while (!_queue.empty())
            _queue.pop();
    }

private:
    std::queue<nsg::command> _queue;
};

class queue_reader : public nsg::reader
{
public:
    queue_reader(std::queue<std::shared_ptr<nsg::command_buffer>>& shared_queue) : _queue(shared_queue) {}

    std::shared_ptr<nsg::command_buffer> read() override
    {
        if (_queue.empty())
            return nullptr;

        auto buffer = _queue.front();
        _queue.pop();
        return buffer;
    }
private:
    std::queue<std::shared_ptr<nsg::command_buffer>>& _queue;
};

class queue_writer : public nsg::writer
{
    public:
    queue_writer(std::queue<std::shared_ptr<nsg::command_buffer>>& shared_queue) : _queue(shared_queue) {}

    void write(const std::shared_ptr<nsg::command_buffer> &buffer) override
    {
        _queue.push(buffer);
    }
private:
    std::queue<std::shared_ptr<nsg::command_buffer>>& _queue;
};

MAXTEST_MAIN
{
    MAXTEST_TEST_CASE(command_test)
    {

    };

    MAXTEST_TEST_CASE(reader_test)
    {
        std::queue<std::shared_ptr<nsg::command_buffer>> shared_queue;
        auto reader = std::make_shared<queue_reader>(shared_queue);
        auto buffer = std::make_shared<queue_buffer>();

        buffer->push({nsg::command::op::add, "node1", "", {{"key", nsg::property("value")}}});
        shared_queue.push(buffer);

        auto read_buffer = reader->read();
        nsg::command cmd;
        read_buffer->pop(cmd);

        MAXTEST_ASSERT(cmd.cmd_type == nsg::command::op::add);
        MAXTEST_ASSERT(cmd.node_id == "node1");
        MAXTEST_ASSERT(cmd.properties["key"].get_type() == nsg::property::type::string);
    };
}