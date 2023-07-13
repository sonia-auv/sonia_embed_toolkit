#pragma once

#include <stdint.h>
#include <cstdlib>
#include "hamming_func.h"

namespace sonia_embed_toolkit
{
    class HammingToolkit
    {
        public:
            static int pack_7_bits_values(uint8_t* unpack_values, size_t size_unpack_values, uint8_t* pack_values, size_t size_pack_values);
            static int unpack_7_bits_values(uint8_t* pack_values, size_t size_pack_values, uint8_t* unpack_values, size_t size_unpack_values);

            static int decode_hamming_74_message(uint8_t* encoded_data, size_t encoded_data_size, uint8_t* decoded_data, size_t decoded_data_size);
            static int encode_hamming_74_message(uint8_t* data, size_t data_size, uint8_t* encoded_data, size_t encoded_data_size);

            static void interlace_message(uint8_t* data, size_t data_size, uint8_t* interlaced_data);
            static void deinterlace_message(uint8_t* data, size_t data_size, uint8_t* interlaced_data);

            static uint8_t get_bit(uint8_t data, uint8_t pos);
    };
}