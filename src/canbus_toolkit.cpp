#include "canbus_toolkit.h"
#include <stdexcept>
#include <iostream>
#include <cstring>

namespace sonia_embed_toolkit
{
    const uint8_t CanBusToolkit::MAX_MSG_SIZE = 128;

    uint8_t CanBusToolkit::convert_to_can(const uint8_t size, const uint8_t* message, uint8_t* can_msg)
    {
        uint8_t num_packets_no_head = (size < 8) ? 1 : size / 8 + ((size % 8 == 0) ? 0 : 1);
        uint8_t num_packets_with_head = ((size + num_packets_no_head) < 8) ? 1 : (size + num_packets_no_head) / 8 + (((size + num_packets_no_head) % 8 == 0) ? 0 : 1);
        if (num_packets_with_head > 15)
        {
            throw std::invalid_argument("Message too big");
        }

        uint8_t msg_counter = 0;
        std::array<uint8_t, 8> tmp_array;
        for (uint8_t i = 0; i < num_packets_with_head; i++)
        {
            tmp_array[0] = (i) << 4 | (num_packets_with_head-1);
            memcpy(&tmp_array[1], &message[msg_counter*7], 7);
            memcpy(&can_msg[i*8], tmp_array.data(), 8);
            msg_counter++;
        }

        return size + num_packets_no_head;
    }

    uint8_t CanBusToolkit::convert_to_can(const uint8_t size, const uint16_t* message, uint8_t* can_msg)
    {
        return CanBusToolkit::convert_to_can(size * sizeof(uint16_t), (uint8_t*)message, can_msg);
    }


} // namespace sonia_embed_toolkit
