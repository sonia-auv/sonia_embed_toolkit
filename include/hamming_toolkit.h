#pragma once

#include <stdint.h>
#include <cstdlib>
#include "hamming_func.h"

namespace sonia_embed_toolkit
{
    class HammingToolkit
    {
    public:
        /**
         * @brief Pack the values
        */
        static int pack_7_bits_values(uint8_t *unpack_values, size_t size_unpack_values, uint8_t *pack_values, size_t size_pack_values);
        static int unpack_7_bits_values(const uint8_t *pack_values, size_t size_pack_values, uint8_t *unpack_values, size_t size_unpack_values);

        /**
         * @brief Decode a message that was encoded using the Hamming 7-4 algorithm.
         * 
         * @param encoded_data      Array of encoded data to decode.
         * @param encoded_data_size Size of encoded data array.
         * @param decoded_data      Array of decoded data.
         * @param decoded_data_size Size of decoded data array.
         * 
         * @return int Returns -1 if `encoded_data_size` is less than 2 times the size of decoded_data_size else returns 0.
        */
        static int decode_hamming_74_message(const uint8_t *encoded_data, size_t encoded_data_size, uint8_t *decoded_data, size_t decoded_data_size);
        
        /**
         * @brief Encode a message using the Hamming 7-4 algorithm. 
         * 
         * @param data              Array of data to encode.
         * @param data_size         Size of data array.
         * @param encoded_data      Array of encoded data.
         * @param encoded_data_size Size of encoded data array.
         * 
         * @return int Return -1 if the encoded_data_size is not at least 2 times data_size else returns 0.
        */
        static int encode_hamming_74_message(const uint8_t *data, size_t data_size, uint8_t *encoded_data, size_t encoded_data_size);

        static uint8_t get_bit(uint8_t data, uint8_t pos);

        /**
         * @brief Interleave encoded data such that all bytes are evenly distributed.
         * 
         * @param data              Data array to interleave.
         * @param data_size         Size of data array.
         * @param interleaved_data  Array of interleaved data.
        */
        static void interleaving_pre_pack(uint8_t *data, size_t data_size, uint8_t *interleaved_data);
        
        /**
         * @brief Interleave encoded data such that all bytes are evenly distributed.
         * 
         * @param data              Deinterleaved data array.
         * @param data_size         Size of data array.
         * @param interleaved_data  Interleaved data array.
        */
        static void deinterleaving_post_depack(uint8_t *data, size_t data_size, uint8_t *interleaved_data);
    };
}