#include <gtest/gtest.h>
#include "rs485_toolkit.h"
#include <stdint.h>

namespace sonia_embed_toolkit
{    
    TEST(RS485ToolkitTest, CONVERT_MSG_NO_BODY)
    {
        uint8_t empty_serial_message[3];
        ASSERT_EQ(RS485Toolkit::convert_message_to_serial(1, empty_serial_message), 0);
        ASSERT_EQ(empty_serial_message[0], RS485Toolkit::START_BYTE);
        ASSERT_EQ(empty_serial_message[1], 1);
        ASSERT_EQ(empty_serial_message[2], 0);
    };

    TEST(RS485ToolkitTest, CONVERT_MSG_SINGLE_VALUE)
    {
        uint8_t single_message[1] = {55};
        uint8_t single_value_serial_message[4];
        ASSERT_EQ(RS485Toolkit::convert_message_to_serial(1, 1, single_message, single_value_serial_message), 0);
        ASSERT_EQ(single_value_serial_message[0], RS485Toolkit::START_BYTE);
        ASSERT_EQ(single_value_serial_message[1], 1);
        ASSERT_EQ(single_value_serial_message[2], 1);
        ASSERT_EQ(single_value_serial_message[3], 55);
    };

    TEST(RS485ToolkitTest, CONVERT_MSG_MULTIVALUE)
    {
        uint8_t two_message[2] = {20, 255};
        uint8_t longer_serial_message[10];
        ASSERT_EQ(RS485Toolkit::convert_message_to_serial(1, 2, two_message, longer_serial_message), 0);
        ASSERT_EQ(longer_serial_message[0], RS485Toolkit::START_BYTE);
        ASSERT_EQ(longer_serial_message[1], 1);
        ASSERT_EQ(longer_serial_message[2], 2);
        ASSERT_EQ(longer_serial_message[3], 20);
        ASSERT_EQ(longer_serial_message[4], 255);
        for (size_t i = 5; i < 10; i++)
        {
            ASSERT_EQ(longer_serial_message[i], NULL);
        }  
    };

    TEST(RS485ToolkitTest, CONVERT_SHORT_MSG)
    {
        uint16_t speed[2] = {1400, 1600};
        uint8_t serial_message[7];
        ASSERT_EQ(RS485Toolkit::convert_message_to_serial(2, 2, speed, serial_message), 0);
        ASSERT_EQ(serial_message[0], RS485Toolkit::START_BYTE);
        ASSERT_EQ(serial_message[1], 2);
        ASSERT_EQ(serial_message[2], 4);
        ASSERT_EQ(serial_message[4]<<8|serial_message[3], 1400);
        ASSERT_EQ(serial_message[6]<<8|serial_message[5], 1600);
    };

    TEST(RS485ToolkitTest, CONVERT_SERIAL)
    {
        uint8_t serial_message[10] = {RS485Toolkit::START_BYTE, 2, 4, 10, 20, 25, 50};
        uint8_t msg[10];
        std::pair<uint8_t, uint8_t> id_size(2, 4);
        ASSERT_EQ(RS485Toolkit::convert_serial_to_message(serial_message, msg), id_size);

        ASSERT_EQ(msg[0], 10);
        ASSERT_EQ(msg[1], 20);
        ASSERT_EQ(msg[2], 25);
        ASSERT_EQ(msg[3], 50);
        ASSERT_EQ(msg[4], NULL);
    }
} // namespace sonia_embed


