#ifndef EVSTREAM_DRIVER_HPP
#define EVSTREAM_DRIVER_HPP

#include "event.hpp"

namespace evstream
{
    /**
     * @brief Abstract base class for event drivers.
     *
     * This class defines the interface for collecting system events.
     */
    class driver
    {
    public:
        /**
         *  @brief Collect the next event.
         *  @param next The event to be filled with the next collected event.
         *  @return true if an event was collected, false if no more events are available.
         */
        virtual bool next(event &next) = 0;

        /**
         *  @brief Enable a specific type of event.
         *  @param type The type of event to enable.
         *  This method indicates to the driver that a given event type should not be discarded.
         */
        virtual void enable(event::type type);

        /**
         *  @brief Disable a specific type of event.
         *  @param type The type of event to disable.
         *  This method indicates to the driver that a given event type can be discarded.
         */
        virtual void disable(event::type type);
    };
}

#endif