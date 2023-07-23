#include <gtest/gtest.h>
#include "rs485_toolkit.h"
#include <stdint.h>

namespace sonia_embed_toolkit
{    
    TEST(RS485ToolkitTest, CONVERT_MSG_NO_BODY)
    {
        uint8_t pack_header_message[RS485Toolkit::PACK_HEADER_SIZE];
        uint8_t header_message[RS485Toolkit::HEADER_SIZE];

        ASSERT_EQ(RS485Toolkit::convert_header_to_serial(1, pack_header_message), 0);
        ASSERT_EQ(RS485Toolkit::convert_serial_to_header(pack_header_message, header_message), 0);

        ASSERT_EQ(header_message[0], RS485Toolkit::START_BYTE);
        ASSERT_EQ(header_message[1], 1);
        ASSERT_EQ(header_message[2], 0);
    };

    TEST(RS485ToolkitTest, CONVERT_MSG_SINGLE_VALUE)
    {
        uint8_t single_message[1] = {55};
        uint8_t packed_single_value_serial_message[RS485Toolkit::PACK_MSG_SIZE];
        uint8_t single_value_serial_message[RS485Toolkit::MAX_MSG_SIZE] = {1};
        ASSERT_EQ(RS485Toolkit::convert_message_to_serial(1, 1, single_message, packed_single_value_serial_message), 0);
        std::pair<size_t, size_t> id_size = RS485Toolkit::convert_serial_to_message(packed_single_value_serial_message, single_value_serial_message);
        ASSERT_EQ(id_size.first, 1);
        ASSERT_EQ(id_size.second, 1);

        ASSERT_EQ(single_value_serial_message[0], single_message[0]);
    };

    TEST(RS485ToolkitTest, CONVERT_MSG_MULTIVALUE)
    {
        uint8_t two_message[2] = {20, 255};
        uint8_t packed_longer_serial_message[RS485Toolkit::PACK_MSG_SIZE];
        uint8_t longer_serial_message[RS485Toolkit::MAX_MSG_SIZE] = {0};
        ASSERT_EQ(RS485Toolkit::convert_message_to_serial(1, 2, two_message, packed_longer_serial_message), 0);
        std::pair<size_t, size_t> id_size = RS485Toolkit::convert_serial_to_message(packed_longer_serial_message, longer_serial_message);
        ASSERT_EQ(id_size.first, 1);
        ASSERT_EQ(id_size.second, 2);

        ASSERT_EQ(longer_serial_message[0], two_message[0]);
        ASSERT_EQ(longer_serial_message[1], two_message[1]);
    };

    TEST(RS485ToolkitTest, CONVERT_SHORT_MSG)
    {
        uint16_t speed[2] = {1400, 1600};
        uint8_t packed_short_value_serial_message[RS485Toolkit::PACK_MSG_SIZE];
        uint8_t short_value_serial_message[RS485Toolkit::MAX_MSG_SIZE] = {1};


        ASSERT_EQ(RS485Toolkit::convert_message_to_serial(2, 2, speed, packed_short_value_serial_message), 0);
        std::pair<size_t, size_t> id_size = RS485Toolkit::convert_serial_to_message(packed_short_value_serial_message, short_value_serial_message);
        ASSERT_EQ(id_size.first, 2);
        ASSERT_EQ(id_size.second, 4);

        ASSERT_EQ(short_value_serial_message[1]<<8|short_value_serial_message[0], 1400);
        ASSERT_EQ(short_value_serial_message[3]<<8|short_value_serial_message[2], 1600);
    };
} // namespace sonia_embed


