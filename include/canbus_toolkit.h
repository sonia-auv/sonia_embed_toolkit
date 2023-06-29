#pragma once

#include <stdint.h>
#include <array>


namespace sonia_embed_toolkit
{   
    class CanBusToolkit
    {
        public:
        static const uint8_t MAX_CAN_SIZE;
        static const uint8_t MAX_MSG_SIZE;
        static uint8_t convert_to_can(const uint8_t msg_size, const uint8_t* message, uint8_t* can_message);
        static uint8_t convert_to_can(const uint8_t msg_size, const uint16_t* message, uint8_t* can_message);
        static uint8_t convert_from_can(const uint8_t can_size, const uint8_t* can_message, uint8_t* message);
    };
}