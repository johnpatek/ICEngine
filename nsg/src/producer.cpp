#include <nsg.hpp>

nsg::producer::producer(const std::shared_ptr<nsg::command_buffer>& command_buffer)
    : _command_buffer(command_buffer) {}

void nsg::producer::add(const std::string& node_id, const std::string& parent_id, const std::unordered_map<std::string, property>& properties) {
    command cmd;
    cmd.cmd_type = nsg::command::op::add;
    cmd.node_id = node_id;
    cmd.parent_id = parent_id;
    cmd.properties = properties;

    _command_buffer->push(cmd);
}

void nsg::producer::remove(const std::string& node_id) {
    command cmd;
    cmd.cmd_type = nsg::command::op::remove;
    cmd.node_id = node_id;

    _command_buffer->push(cmd);
}

void nsg::producer::update(const std::string& node_id, const std::unordered_map<std::string, property>& properties) {
    command cmd;
    cmd.cmd_type = nsg::command::op::update;
    cmd.node_id = node_id;
    cmd.properties = properties;

    _command_buffer->push(cmd);
}

void nsg::producer::move(const std::string& node_id, const std::string& parent_id) {
    command cmd;
    cmd.cmd_type = nsg::command::op::move;
    cmd.node_id = node_id;
    cmd.parent_id = parent_id;

    _command_buffer->push(cmd);
}