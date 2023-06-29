#include "canbus_toolkit.h"
#include <stdexcept>
#include <iostream>
#include <cstring>

namespace sonia_embed_toolkit
{
    const uint8_t CanBusToolkit::MAX_CAN_SIZE = 128;
    const uint8_t CanBusToolkit::MAX_MSG_SIZE = 112;

    uint8_t CanBusToolkit::convert_to_can(const uint8_t size, const uint8_t* message, uint8_t* can_msg)
    {
        if (size > CanBusToolkit::MAX_MSG_SIZE)
        {
            return 0;
        }
        uint8_t num_packets_with_head = (size < 7) ? 1 : (size / 7) + ((size % 7 == 0) ? 0 : 1);

        uint8_t msg_counter = 0;
        std::array<uint8_t, 8> tmp_array;
        for (uint8_t i = 0; i < num_packets_with_head; i++)
        {
            tmp_array[0] = (i) << 4 | (num_packets_with_head - 1);
            memcpy(&tmp_array[1], &message[msg_counter * 7], 7);
            memcpy(&can_msg[i * 8], tmp_array.data(), 8);
            msg_counter++;
        }

        return size + num_packets_with_head;
    }

    uint8_t CanBusToolkit::convert_to_can(const uint8_t size, const uint16_t* message, uint8_t* can_msg)
    {
        return CanBusToolkit::convert_to_can(size * sizeof(uint16_t), (uint8_t*)message, can_msg);
    }

    uint8_t CanBusToolkit::convert_from_can(const uint8_t can_size, const uint8_t* can_message, uint8_t* message)
    {
        uint8_t num_packets = (can_size < 8) ? 1 : can_size / 8 + ((can_size % 8 == 0) ? 0 : 1);
        uint8_t msg_size = can_size - num_packets;
        // TODO add order correction
        size_t cur_size = can_size;
        std::array<uint8_t, CanBusToolkit::MAX_CAN_SIZE> transfer_array;
        std::array<uint8_t, CanBusToolkit::MAX_CAN_SIZE> tmp_array;
        memcpy(transfer_array.data(), can_message, can_size);
        while (cur_size > 0)
        {
            size_t index = transfer_array[0] >> 4;
            size_t total = transfer_array[0] & 0b1111;
            if (index == total)
            {
                size_t remainder = can_size % 8;
                memcpy(&message[index * 7], &transfer_array[1], remainder - 1);
                cur_size -= remainder;
                memcpy(tmp_array.data(), &transfer_array[remainder], cur_size);
            }
            
            else
            {
                memcpy(&message[index * 7], &transfer_array[1], 7);
                cur_size -= 8;
                memcpy(tmp_array.data(), &transfer_array[8], cur_size);
            }
            if (cur_size > 0)
            {
                memcpy(transfer_array.data(), tmp_array.data(), cur_size);
            }
        }
        return msg_size;
    }


} // namespace sonia_embed_toolkit
