#ifndef NSG_CONSUMER_HPP
#define NSG_CONSUMER_HPP

#include "common.hpp"

namespace nsg 
{
    /**
     * @brief The consumer class for handling commands from a command buffer.
     *
     * This class defines methods to handle different types of commands such as 
     * add, remove, update, and move. It makes no assumptions about the scheduling 
     * policy, and includes no synchronization mechanisms for concurrent consumers 
     * acting on the same node. While theoretically possible, and possibly useful, 
     * any support for parallel execution is not included in this interface.
     */
    class consumer
    {
    public:

        /**
         * @brief Handles an add command.
         * @param node_id The ID of the node to be added.
         * @param parent_id The ID of the parent node.
         * @param properties A map of properties associated with the node.
         * This method is called when an add command is received.
         */
        virtual void handle_add(const std::string &node_id, const std::string &parent_id, const std::unordered_map<std::string, property>& properties) = 0;

        /**
         * @brief Handles a remove command.
         * @param node_id The ID of the node to be removed.
         * This method is called when a remove command is received.
         */
        virtual void handle_remove(const std::string &node_id) = 0;

        /**
         * @brief Handles an update command.
         * @param node_id The ID of the node to be updated.
         * @param properties A map of properties to update for the node.
         * This method is called when an update command is received.
         */
        virtual void handle_update(const std::string &node_id, const std::unordered_map<std::string, property>& properties) = 0;

        /**
         * @brief Handles a move command.
         * @param node_id The ID of the node to be moved.
         * @param parent_id The ID of the new parent node.
         * This method is called when a move command is received.
         */
        virtual void handle_move(const std::string &node_id, const std::string &parent_id) = 0;
    };

}

#endif