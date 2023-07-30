#pragma once

#include <stdint.h>
#include <array>

namespace sonia_embed_toolkit
{
    class CanBusToolkit
    {
    public:
        /**
         * @brief Max size for a canbus message.
        */
        static const uint8_t MAX_CAN_SIZE;
        /**
         * @brief Max size for a regular message.
        */
        static const uint8_t MAX_MSG_SIZE;

        /**
         * @brief Convert Message to Can Message format.
         * 
         * @param msg_size      Size of the message to convert.
         * @param message       Message to convert.
         * @param can_message   Resulting array after conversion.
         * 
         * @param uint8_t       Returns the size of the can_message.
        */
        static uint8_t convert_to_can(const uint8_t msg_size, const uint8_t *message, uint8_t *can_message);
        static uint8_t convert_to_can(const uint8_t msg_size, const uint16_t *message, uint8_t *can_message);

        /**
         * @brief Convert Can Message to regular message format.
         * 
         * @param can_size      Size of the Can message.
         * @param can_message   Can Message array.
         * @param message       Converted message array.
         * 
         * @return uint8_t      Size of converted message array.
        */
        static uint8_t convert_from_can(const uint8_t can_size, const uint8_t *can_message, uint8_t *message);
    };
}