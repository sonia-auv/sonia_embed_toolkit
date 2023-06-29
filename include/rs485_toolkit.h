#pragma once

#include <stdint.h>
#include <array>


namespace sonia_embed_toolkit
{
    
    class RS485Toolkit
    {
        public:
        static const uint8_t START_BYTE;
        static const uint8_t HEADER_SIZE;

        static uint8_t convert_message_to_serial(const uint8_t id, const uint8_t size, const uint8_t* message, uint8_t* serial_message);
        static uint8_t convert_message_to_serial(const uint8_t id, const uint8_t size, const uint16_t* message, uint8_t* serial_message);
        static uint8_t convert_message_to_serial(const uint8_t id, uint8_t* serial_message, const uint8_t size = 0);

        static std::pair<uint8_t, uint8_t> convert_serial_to_message(const uint8_t* serial_msg, uint8_t* message);

        private:
    };
}