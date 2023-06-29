#include <gtest/gtest.h>
#include "canbus_toolkit.h"
#include <stdint.h>


/*
static CanBusStorage convert_to_can(const uint8_t id, const uint8_t size, const uint8_t* message);

*/
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
        ASSERT_EQ(CanBusToolkit::convert_to_can(16, msg_to_send, can_msg), 18);

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
    };

    TEST(CanBusToolkitTest, MSG_TOO_BIG)
    {
        uint8_t msg_to_send[106] = {0};        
        uint8_t can_msg[120];
        ASSERT_NO_THROW(CanBusToolkit::convert_to_can(106, msg_to_send, can_msg));
        std::cout << (int)can_msg[121] <<std::endl;
        ASSERT_THROW(CanBusToolkit::convert_to_can(107, msg_to_send, can_msg), std::invalid_argument);
    };
}