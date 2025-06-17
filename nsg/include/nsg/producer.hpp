#ifndef NSG_PRODUCER_HPP
#define NSG_PRODUCER_HPP

#include "common.hpp"

namespace nsg
{
    /**
     *  @brief The producer class for recording commands into a command buffer.
     */
    class producer
    {
    public:
        /**
         *  @brief Constructs a producer with the given command buffer.
         *  @param command_buffer A shared pointer to the command buffer where commands will be recorded.
         */
        producer(const std::shared_ptr<command_buffer> &command_buffer);

        /**
         *  @brief Default destructor for the producer.
         */
        ~producer() = default;

        /**
         *  @brief Adds a command to the command buffer.
         *  @param node_id The ID of the node to be added.
         *  @param parent_id The ID of the parent node.
         *  @param properties A map of properties associated with the node.
         *  This method records an add command into the command buffer.
         */
        void add(const std::string &node_id, const std::string &parent_id, const std::unordered_map<std::string, property> &properties);

        /**
         *  @brief Removes a command from the command buffer.
         *  @param node_id The ID of the node to be removed.
         *  This method records a remove command into the command buffer.
         */
        void remove(const std::string &node_id);

        /**
         *  @brief Updates a command in the command buffer.
         *  @param node_id The ID of the node to be updated.
         *  @param properties A map of properties to update for the node.
         */
        void update(const std::string &node_id, const std::unordered_map<std::string, property> &properties);
        
        /**
         *  @brief Moves a command in the command buffer.
         *  @param node_id The ID of the node to be moved.
         *  @param parent_id The ID of the new parent node.
         *  This method records a move command into the command buffer.
         */
        void move(const std::string &node_id, const std::string &parent_id);

    private:
        std::shared_ptr<command_buffer> _command_buffer;
    };
}

#endif