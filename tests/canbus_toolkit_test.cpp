#include <gtest/gtest.h>
#include "canbus_toolkit.h"
#include <stdint.h>


namespace sonia_embed_toolkit
{
    TEST(CanBusToolkitTest, SEND_MSG_ONE_PACKET)
    {
        uint8_t msg_to_send[2] = {55, 110};
        uint8_t can_msg[CanBusToolkit::MAX_MSG_SIZE];
        ASSERT_EQ(CanBusToolkit::convert_to_can(2, msg_to_send, can_msg), 3);

        ASSERT_EQ(can_msg[0], 0);
        ASSERT_EQ(can_msg[1], 55);
        ASSERT_EQ(can_msg[2], 110);
    };

    TEST(CanBusToolkitTest, SEND_LARGE_BYTE_MSG)
    {
        uint8_t msg_to_send[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        uint8_t can_msg[CanBusToolkit::MAX_MSG_SIZE];
        ASSERT_EQ(CanBusToolkit::convert_to_can(16, msg_to_send, can_msg), 19);

        uint8_t assert_counter = 0;
        for (size_t i = 0; i < 3; i++)
        {
            ASSERT_EQ(can_msg[i*8], (i)<<4|2);
            for (size_t j = 1; j < 8; j++)
            {
                ASSERT_EQ(can_msg[i*8+j], msg_to_send[assert_counter]);
                assert_counter++;
            }
        }
    };

    TEST(CanBusToolkitTest, SEND_SHORT_MSG)
    {
        uint16_t msg_to_send[2] = {1400, 1600};
        uint8_t can_msg[CanBusToolkit::MAX_MSG_SIZE];
        ASSERT_EQ(CanBusToolkit::convert_to_can(2, msg_to_send, can_msg), 5);
        ASSERT_EQ(can_msg[0], 0);
        ASSERT_EQ(can_msg[0], 0);
        ASSERT_EQ(can_msg[0], 0);
    };

    TEST(CanBusToolkitTest, MSG_TOO_BIG)
    {
        uint8_t msg_to_send[CanBusToolkit::MAX_MSG_SIZE];        
        for (size_t i = 0; i < CanBusToolkit::MAX_MSG_SIZE; i++)
        {
            msg_to_send[i] = i;
        }
        
        uint8_t can_msg[CanBusToolkit::MAX_CAN_SIZE];
        ASSERT_EQ(CanBusToolkit::convert_to_can(CanBusToolkit::MAX_MSG_SIZE, msg_to_send, can_msg), 128);
        uint8_t alt_counter = 0;
        for (size_t index = 0; index < CanBusToolkit::MAX_CAN_SIZE; index++)
        {
            if (index % 8 == 0)
            {
                ASSERT_EQ(can_msg[index]>>4, index / 8);
                ASSERT_EQ(can_msg[index]&0b1111, 15);
            }
            else
            {
                ASSERT_EQ(can_msg[index], alt_counter);
                alt_counter++;
            }
            
        }
        
        ASSERT_EQ(CanBusToolkit::convert_to_can(113, msg_to_send, can_msg), 0);
    };

    TEST(CanBusToolkitTest, RECV_SINGLE_CAN_MSG)
    {
        uint8_t can_msg[3] = {0, 55, 110};
        uint8_t msg[CanBusToolkit::MAX_MSG_SIZE];
        ASSERT_EQ(CanBusToolkit::convert_from_can(3, can_msg, msg), 2);
        ASSERT_EQ(msg[0], 55);
        ASSERT_EQ(msg[1], 110);
        ASSERT_EQ(msg[2], NULL);
    }

    TEST(CanBusToolkitTest, RECV_LARGE_CAN_MSG)
    {
        uint8_t can_msg[19] = {0<<4|2, 1, 2, 3, 4, 5, 6, 7, 1<<4|2, 8, 9, 10, 11, 12, 13, 14, 2<<4|2, 15, 16};
        uint8_t msg[CanBusToolkit::MAX_MSG_SIZE];
        ASSERT_EQ(CanBusToolkit::convert_from_can(19, can_msg, msg), 16);
        for (size_t i = 0; i < 16; i++)
        {
            ASSERT_EQ(msg[i], i+1);
        }
    };

    TEST(CanBusToolkitTest, RECV_CAN_MSG_BAD_ORDER)
    {
        uint8_t can_msg[19] = {2<<4|2, 15, 16, 0<<4|2, 1, 2, 3, 4, 5, 6, 7, 1<<4|2, 8, 9, 10, 11, 12, 13, 14};
        uint8_t msg[CanBusToolkit::MAX_MSG_SIZE];
        ASSERT_EQ(CanBusToolkit::convert_from_can(19, can_msg, msg), 16);
        for (size_t i = 0; i < 16; i++)
        {
            ASSERT_EQ(msg[i], i+1);
        }
    }
}