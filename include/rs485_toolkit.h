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
        static const uint8_t MAX_MSG_SIZE;
        static const uint8_t PACK_HEADER_SIZE;
        static const uint8_t PACK_MSG_SIZE;

        /**
         * @brief Convert a message into serial format.
         * 
         * @param id                ID that will be encoded into the message.
         * @param size              Size of the message.
         * @param message           The message array to convert.
         * @param serial_message    The array that will be filled with the serial message.
         * @return size_t           The size of the serial_message.
         */
        static size_t convert_message_to_serial(const uint8_t id, const uint8_t size, const uint8_t *message, uint8_t *serial_message);
        static size_t convert_message_to_serial(const uint8_t id, const uint8_t size, const uint16_t *message, uint8_t *serial_message);

        /**
         * @brief Convert the header into a serial message.
         * 
         * @param id                ID that will be encoded into the message.
         * @param header_tram       Size of the message.
         * @param size              The message array to convert.
         */
        static void convert_header_to_serial(const uint8_t id, uint8_t *header_tram, const uint8_t size = 0);


        /**
         * @brief Convert a serial message to regular message.
         * 
         * @param serial_msg    The message to convert.
         * @param message       The converted message.
         * @return std::pair<uint8_t, uint8_t> Returns a pair with the first being the message id 
         *                                     and the second being the size.
        */
        static std::pair<uint8_t, uint8_t> convert_serial_to_message(const uint8_t *serial_msg, uint8_t *message);

        /**
         * @brief Convert a serial header to regular header.
         * 
         * @param serial_msg    The message to convert.
         * @param header        The converted header.
        */
        static void convert_serial_to_header(const uint8_t *serial_msg, uint8_t *header);
    };
}