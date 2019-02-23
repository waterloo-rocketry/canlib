#include "can_buffering_layer.h"
#include "buffer_received_can_message.h"
#include <stdio.h>

#define REPORT_FAIL(x) printf("%s: Fail: %s\n", __FUNCTION__, x)

//Utility function, returns true if both messages are the same, else false
static bool can_msg_compare(const can_msg_t *s, const can_msg_t *p)
{
    if (s->sid != p->sid) {
        return false;
    } else if (s->data_len != p->data_len) {
        return false;
    }
    uint8_t i;
    for (i = 0; i < s->data_len; ++i) {
        if (s->data[i] != p->data[i]) {
            return false;
        }
    }
    return true;
}

//Tries to buffer a single message, then take that message out, and make
//sure that it's the same message
static bool test_buffer_single_message(void)
{
    uint8_t memory[100];
    receive_buffer_init((void *) memory, sizeof(memory));
    can_msg_t rcv, send = {
        .sid = 0x7ab,
        .data_len = 4,
        .data = {
            [0] = 0xab,
            [1] = 0xcd,
            [2] = 0xef,
            [3] = 0xef,
        },
    };

    if (!available_received_can_message_space()) {
        REPORT_FAIL("no space in newly initialized receive buffer");
        return false;
    }
    buffer_received_can_message(&send);
    if (!buffered_received_can_message_available()) {
        REPORT_FAIL("no available message after putting one in there");
        return false;
    }
    if (!get_buffered_can_message(&rcv)) {
        REPORT_FAIL("can't get the message we just buffered");
        return false;
    }
    if (buffered_received_can_message_available()) {
        REPORT_FAIL("message still available after reading it");
        return false;
    }
    if (!can_msg_compare(&send, &rcv)) {
        REPORT_FAIL("the message we put in is not the message we took out");
        return false;
    }
    return true;
}

//buffer 10 CAN messages, then pull them all out and make sure it works
static bool test_buffer_ten_messages(void)
{
    //a single element takes up 14 bytes, so to do 10 you need 140 bytes
    uint8_t memory[14 * 10];
    receive_buffer_init((void *) memory, sizeof(memory));

    uint8_t i;
    can_msg_t rcv, send = {
        .sid = 0,
        .data_len = 2,
        .data = {
            [0] = 72,
            [1] = 49,
        },
    };
    for (i = 0; i < 10; ++i) {
        send.sid = i;
        buffer_received_can_message(&send);
    }
    //confirm that we are all out of room
    if (available_received_can_message_space()) {
        REPORT_FAIL("buffer full but says it has space");
        return false;
    }
    for (i = 0; i < 10; ++i) {
        if (!buffered_received_can_message_available()) {
            REPORT_FAIL("it says there are no messages left");
            return false;
        }
        send.sid = i;
        if (!get_buffered_can_message(&rcv)) {
            REPORT_FAIL("it says we couldn't get a message");
            return false;
        }
        if (!can_msg_compare(&rcv, &send)) {
            REPORT_FAIL("it returned the wrong CAN value");
            return false;
        }
    }
    return true;
}

//make sure that when you fill the buffer, it's smart enough not to write more
//bytes past the range of memory that it was given
static bool test_buffer_doesnt_overrun()
{
    //each element is 14 bytes long. Allocate 29 bytes and make sure the last
    //byte is never changed
    uint8_t memory[29];
    memory[28] = 44;
    receive_buffer_init((void *) memory, 28);
    can_msg_t test = {
        .sid = 0x7FF,
        .data_len = 8,
        .data = {
            [0] = 0xff,
            [1] = 0xff,
            [2] = 0xff,
            [3] = 0xff,
            [4] = 0xff,
            [5] = 0xff,
            [6] = 0xff,
            [7] = 0xff,
        },
    };
    uint8_t i;
    for (i = 0; i < 3; ++i) {
        buffer_received_can_message(&test);
    }
    if (memory[28] != 44) {
        REPORT_FAIL("it overwrote the next byte of memory");
        return false;
    }
    return true;
}

bool test_can_buffering_layer(void)
{
    if (!test_buffer_single_message()) {
        return false;
    }
    if (!test_buffer_ten_messages()) {
        return false;
    }
    if (!test_buffer_doesnt_overrun()) {
        return false;
    }
    return true;
}
