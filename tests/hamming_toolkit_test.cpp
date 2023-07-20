#include <gtest/gtest.h>
#include "hamming_toolkit.h"
#include <stdint.h>
#include <cmath>


namespace sonia_embed_toolkit
{    
    TEST(HammingToolkitTest, HAMMING_DECODE_SIZE_CHECK)
    {
        uint8_t encoded_data[11];
        uint8_t decoded_data[5];

        ASSERT_EQ(HammingToolkit::decode_hamming_74_message(encoded_data,11,decoded_data,5), -1);
    };

    TEST(HammingToolkitTest, HAMMING_ENCODE_SIZE_CHECK)
    {
        uint8_t encoded_data[11];
        uint8_t data[11];
        //if encoded_data is not at leat twice the size of data, return -1
        ASSERT_EQ(HammingToolkit::encode_hamming_74_message(data,11,encoded_data,11), -1);
    };

    TEST(HammingToolkitTest, INTERLEAVE)
    {
        uint8_t data[5] = { 0x1, 0x7F, 0x70, 0x0F, 0x55 };
        uint8_t deinterleave_data[5];

        uint8_t interleave_data[5];

        HammingToolkit::interleaving_pre_pack(data, 5, interleave_data);
        HammingToolkit::deinterleaving_post_depack(deinterleave_data, 5, interleave_data);

        for (int i = 0; i < 5; i++)
        {
            ASSERT_EQ(data[i], deinterleave_data[i]);
        }
    };

    TEST(HammingToolkitTest, PACKING)
    {
        uint8_t data_source[8] = { 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00 };
        uint8_t storage[7] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

        uint8_t result[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

        HammingToolkit::pack_7_bits_values(data_source, 8, storage, 7);
        HammingToolkit::unpack_7_bits_values(storage, 7, result, 8);

        for (int i = 0; i < 8; i++)
        {
            ASSERT_EQ(data_source[i], result[i]);
        }
    };

    TEST(HammingToolkitTest, HAMMING)
    {


        const size_t SIZE_MESSAGE = 11;
        const size_t SIZE_ENCODED_DATA = SIZE_MESSAGE * 2; //number of group of 4 bits 
        const size_t SIZE_DATA_PACK = (size_t)ceil((double)(SIZE_ENCODED_DATA * 7)/8);

        char message[SIZE_MESSAGE + 1] = "01234567890";
        uint8_t encoded_data[SIZE_ENCODED_DATA] = { 0 };
        uint8_t encoded_data_interleaved[SIZE_ENCODED_DATA] = { 0 };
        uint8_t* encoded_data_interleaved_pack = (uint8_t*)malloc(SIZE_DATA_PACK * sizeof(uint8_t));
        uint8_t encoded_data_unpack_interleaved[SIZE_ENCODED_DATA] = { 0 };
        uint8_t encoded_data_unpack_deinterlaced[SIZE_ENCODED_DATA] = { 0 };

        char decoded_message[SIZE_MESSAGE + 1];

        HammingToolkit::encode_hamming_74_message((uint8_t*)message, SIZE_MESSAGE, encoded_data, SIZE_ENCODED_DATA);
        HammingToolkit::interleaving_pre_pack(encoded_data, SIZE_ENCODED_DATA, encoded_data_interleaved);
        HammingToolkit::pack_7_bits_values(encoded_data_interleaved, SIZE_ENCODED_DATA, encoded_data_interleaved_pack, SIZE_DATA_PACK);
        
        //force data corruption
        encoded_data_interleaved_pack[0] ^= 0xFF;
        encoded_data_interleaved_pack[1] ^= 0xFF;
        encoded_data_interleaved_pack[2] ^= 0x3F;

        HammingToolkit::unpack_7_bits_values(encoded_data_interleaved_pack, SIZE_DATA_PACK, encoded_data_unpack_interleaved, SIZE_ENCODED_DATA);
        HammingToolkit::deinterleaving_post_depack(encoded_data_unpack_deinterlaced, SIZE_ENCODED_DATA, encoded_data_unpack_interleaved);
        HammingToolkit::decode_hamming_74_message(encoded_data_unpack_deinterlaced, SIZE_ENCODED_DATA, (uint8_t*)decoded_message, SIZE_MESSAGE);

        for (size_t i = 0; i < SIZE_MESSAGE; i++)
        {
            ASSERT_EQ(message[i], decoded_message[i]);
        }
    };

} // namespace sonia_embed