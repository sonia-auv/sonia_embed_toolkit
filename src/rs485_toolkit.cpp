#include "rs485_toolkit.h"
#include <cstring>
#include <iostream>
#include <array>

namespace sonia_embed_toolkit
{
    const uint8_t RS485Toolkit::START_BYTE = 0x55;
    const uint8_t RS485Toolkit::HEADER_SIZE = 3;
    
    uint8_t RS485Toolkit::convert_message_to_serial(const uint8_t id, const uint8_t size, const uint8_t* message, uint8_t* serial_message)
    {
        RS485Toolkit::convert_message_to_serial(id, serial_message, size);
        
        // TODO build message...
        
        memcpy(&serial_message[3], message, size);
        
        // with interleaving
        return 0;
    }

    uint8_t RS485Toolkit::convert_message_to_serial(const uint8_t id, const uint8_t size, const uint16_t* message, uint8_t* serial_message)
    {
        RS485Toolkit::convert_message_to_serial(id, size * sizeof(uint16_t), (uint8_t*)message, serial_message);
        return 0;
    }

    uint8_t RS485Toolkit::convert_message_to_serial(const uint8_t id, uint8_t* serial_message, const uint8_t size)
    {
        uint8_t header[RS485Toolkit::HEADER_SIZE] = {RS485Toolkit::START_BYTE, id, size};

        memcpy(serial_message, &header, 3);

        return 0;
    }

    std::pair<uint8_t, uint8_t> RS485Toolkit::convert_serial_to_message(const uint8_t* serial_msg, uint8_t* message)
    {
        // decode header
        uint8_t header[3];
        memcpy(&header, serial_msg, 3);

        //TODO Decode message

        memcpy(message, &serial_msg[3], header[2]);
        std::pair<uint8_t, uint8_t> id_size(header[1], header[2]);
        return id_size;
    }

} // namespace sonia_embed


