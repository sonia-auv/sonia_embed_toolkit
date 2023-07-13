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

    TEST(HammingToolkitTest, INTERLACE)
    {
        char message[11] = "0123456789";
        char deinterlace_message_data[11];
        
        uint8_t interlace_data[10];

        HammingToolkit::interlace_message((uint8_t *)message, 10, interlace_data);
        HammingToolkit::deinterlace_message((uint8_t*)deinterlace_message_data, 10, interlace_data);

        deinterlace_message_data[10] = '\0';

        for (int i = 0; i < 10; i++)
        {
            ASSERT_EQ(message[i], deinterlace_message_data[i]);
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
        const size_t SIZE_ENCODED_DATA = SIZE_MESSAGE * 2;
        const size_t SIZE_DATA_PACK = (size_t)ceil((double)(SIZE_ENCODED_DATA * 7)/8);

        char message[SIZE_MESSAGE + 1] = "01234567890";
        uint8_t encoded_data[SIZE_ENCODED_DATA] = { 0 };
        uint8_t* encoded_data_pack = (uint8_t*)malloc(SIZE_DATA_PACK * sizeof(uint8_t));
        uint8_t* encoded_data_pack_interlaced = (uint8_t*)malloc(SIZE_DATA_PACK * sizeof(uint8_t));
        uint8_t* encoded_data_pack_deinterlaced = (uint8_t*)malloc(SIZE_DATA_PACK * sizeof(uint8_t));
        uint8_t encoded_data_unpack[SIZE_ENCODED_DATA] = { 0 };
        char decoded_message[SIZE_MESSAGE + 1];

        /*char message[12] = "01234567890";
        uint8_t encoded_data[22] = {0};
        uint8_t encoded_data_pack[20] = {0};
        uint8_t encoded_data_pack_interlaced[20];
        uint8_t encoded_data_pack_deinterlaced[20];
        uint8_t encoded_data_unpack[22] = {0};
        char decoded_message[12];*/

        HammingToolkit::encode_hamming_74_message((uint8_t*)message, SIZE_MESSAGE, encoded_data, SIZE_ENCODED_DATA);
        HammingToolkit::pack_7_bits_values(encoded_data, SIZE_ENCODED_DATA, encoded_data_pack, SIZE_ENCODED_DATA);
        HammingToolkit::interlace_message(encoded_data_pack, SIZE_ENCODED_DATA, encoded_data_pack_interlaced);
        

        //force data corruption
        encoded_data_pack_interlaced[4] ^= 0xFF;
        encoded_data_pack_interlaced[5] ^= 0xFF;

        HammingToolkit::deinterlace_message(encoded_data_pack_deinterlaced, SIZE_ENCODED_DATA, encoded_data_pack_interlaced);
        HammingToolkit::unpack_7_bits_values(encoded_data_pack_deinterlaced, SIZE_ENCODED_DATA, encoded_data_unpack, SIZE_ENCODED_DATA);
        HammingToolkit::decode_hamming_74_message(encoded_data_unpack, SIZE_ENCODED_DATA, (uint8_t*)decoded_message, SIZE_MESSAGE);

        for (size_t i = 0; i < SIZE_MESSAGE; i++)
        {
            ASSERT_EQ(message[i],decoded_message[i]);
        }
    };

} // namespace sonia_embed