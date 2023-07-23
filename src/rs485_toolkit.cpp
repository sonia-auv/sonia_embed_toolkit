#include <cstring>
#include <iostream>
#include <array>
#include <cmath>

#include "rs485_toolkit.h"
#include "hamming_toolkit.h"

namespace sonia_embed_toolkit
{
    const uint8_t RS485Toolkit::START_BYTE = 0x55;
    const uint8_t RS485Toolkit::HEADER_SIZE = 3;
    const uint8_t RS485Toolkit::MAX_MSG_SIZE = 112;
    const uint8_t RS485Toolkit::PACK_HEADER_SIZE = (size_t)ceil((double)(RS485Toolkit::HEADER_SIZE*2*7)/8);
    const uint8_t RS485Toolkit::PACK_MSG_SIZE = (size_t)ceil((double)(RS485Toolkit::MAX_MSG_SIZE*2*7)/8);
    
    uint8_t RS485Toolkit::convert_message_to_serial(const uint8_t id, const uint8_t size, const uint8_t* message, uint8_t* serial_message)
    {
        uint8_t header_tram[PACK_HEADER_SIZE];
        RS485Toolkit::convert_header_to_serial(id, header_tram, size);
        memcpy(serial_message, header_tram, RS485Toolkit::PACK_HEADER_SIZE);
        

        uint8_t msg_tram[RS485Toolkit::MAX_MSG_SIZE*2];
        uint8_t msg_tram_interleaved[RS485Toolkit::MAX_MSG_SIZE*2];
        uint8_t packed_msg[PACK_MSG_SIZE];
        HammingToolkit::encode_hamming_74_message(message, size, msg_tram, size*2);
        HammingToolkit::interleaving_pre_pack(msg_tram, size*2, msg_tram_interleaved);
        HammingToolkit::pack_7_bits_values(msg_tram_interleaved, size*2, packed_msg, PACK_MSG_SIZE);

        memcpy(&serial_message[RS485Toolkit::PACK_HEADER_SIZE], packed_msg, PACK_MSG_SIZE);
        return 0;
    }

    uint8_t RS485Toolkit::convert_message_to_serial(const uint8_t id, const uint8_t size, const uint16_t* message, uint8_t* serial_message)
    {
        RS485Toolkit::convert_message_to_serial(id, size * sizeof(uint16_t), (uint8_t*)message, serial_message);
        return 0;
    }

    size_t RS485Toolkit::convert_header_to_serial(const uint8_t id, uint8_t* header_tram, const uint8_t size)
    {
        uint8_t header[RS485Toolkit::HEADER_SIZE] = {RS485Toolkit::START_BYTE, id, size};
        uint8_t enc_header[RS485Toolkit::HEADER_SIZE*2];
        uint8_t inter_header[RS485Toolkit::HEADER_SIZE*2];
        uint8_t packed_header[RS485Toolkit::PACK_HEADER_SIZE];
        // Encode 
        HammingToolkit::encode_hamming_74_message(header, RS485Toolkit::HEADER_SIZE, enc_header, RS485Toolkit::HEADER_SIZE*2);
        HammingToolkit::interleaving_pre_pack(enc_header, RS485Toolkit::HEADER_SIZE*2, inter_header);
        HammingToolkit::pack_7_bits_values(inter_header, RS485Toolkit::HEADER_SIZE*2, packed_header, RS485Toolkit::PACK_HEADER_SIZE);

        memcpy(header_tram, &packed_header, RS485Toolkit::PACK_HEADER_SIZE);

        return 0;
    }

    std::pair<uint8_t, uint8_t> RS485Toolkit::convert_serial_to_message(const uint8_t* serial_msg, uint8_t* message)
    {
        // decode header
        uint8_t header[HEADER_SIZE];
        RS485Toolkit::convert_serial_to_header(serial_msg, header);

        //TODO Decode message

        uint8_t interleaved_msg[MAX_MSG_SIZE*2];
        HammingToolkit::unpack_7_bits_values(&serial_msg[PACK_HEADER_SIZE], PACK_MSG_SIZE, interleaved_msg, header[2]*2);
        uint8_t enc_msg[MAX_MSG_SIZE*2];
        HammingToolkit::deinterleaving_post_depack(enc_msg, header[2]*2, interleaved_msg);

        HammingToolkit::decode_hamming_74_message(enc_msg, header[2]*2, message, header[2]);

        std::pair<uint8_t, uint8_t> id_size(header[1], header[2]);
        return id_size;
    }

    size_t RS485Toolkit::convert_serial_to_header(const uint8_t* serial_msg, uint8_t* header)
    {
        uint8_t packed_header[PACK_HEADER_SIZE];
        memcpy(packed_header, serial_msg, PACK_HEADER_SIZE);

        uint8_t interleaved_header[HEADER_SIZE*2];
        HammingToolkit::unpack_7_bits_values(packed_header, PACK_HEADER_SIZE, interleaved_header, HEADER_SIZE*2);
        uint8_t enc_header[HEADER_SIZE*2];
        HammingToolkit::deinterleaving_post_depack(enc_header, HEADER_SIZE*2, interleaved_header);

        HammingToolkit::decode_hamming_74_message(enc_header, HEADER_SIZE*2, header, HEADER_SIZE);

        return 0;
    }

} // namespace sonia_embed


