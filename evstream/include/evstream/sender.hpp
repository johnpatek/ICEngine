#ifndef EVSTREAM_SENDER_HPP
#define EVSTREAM_SENDER_HPP

#include "event.hpp"

namespace evstream
{
    /**
     * @brief The sender interface for sending events.
     *
     * This interface defines a method to send events to a destination.
     */
    class sender
    {
    public:
        /**
         * @brief Sends an event.
         *
         * This method sends the provided event to the output destination.
         *
         * @param event The event that will be sent.
         */
        virtual void send(const event &event) = 0;
    };
}

#endif