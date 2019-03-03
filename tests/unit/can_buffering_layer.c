#include "can_buffering_layer.h"
#include "can_rcv_buffer.h"
#include <stdio.h>

//if this test is running on hardware, don't actually print anything
#ifndef NO_PRINTF
#define REPORT_FAIL(x) printf("%s: Fail: %s\n", __FUNCTION__, x)
#else
#define REPORT_FAIL(x)
#endif

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
    rcvb_init((void *) memory, sizeof(memory));
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

    if (rcvb_is_full()) {
        REPORT_FAIL("no space in newly initialized receive buffer");
        return false;
    }
    rcvb_push_message(&send);
    if (rcvb_is_empty()) {
        REPORT_FAIL("no available message after putting one in there");
        return false;
    }
    if (!rcvb_pop_message(&rcv)) {
        REPORT_FAIL("can't get the message we just buffered");
        return false;
    }
    if (!rcvb_is_empty()) {
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
    //a single element takes up 14 bytes on a 64 bit laptop, so to do 10 you
    //need 140 bytes. On an 8 bit micro, an element takes 12 bytes.
    uint8_t memory[14 * 10];
    rcvb_init((void *) memory, sizeof(memory));

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
        rcvb_push_message(&send);
    }
    for (i = 0; i < 10; ++i) {
        if (rcvb_is_empty()) {
            REPORT_FAIL("it says there are no messages left");
            return false;
        }
        send.sid = i;
        if (!rcvb_pop_message(&rcv)) {
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
    memory[28] = 44; //magic number
    rcvb_init((void *) memory, 28);
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
        rcvb_push_message(&test);
    }
    if (memory[28] != 44) {
        REPORT_FAIL("it overwrote the next byte of memory");
        return false;
    }
    return true;
}

static bool test_buffer_overflow()
{
    uint8_t memory[100];
    rcvb_init((void *) memory, sizeof(memory));
    can_msg_t msg = {
        .data_len = 0,
        .sid = 0x7FF,
    };
    while (!rcvb_is_full()) {
        rcvb_push_message(&msg);
    }
    if (rcvb_has_overflowed()) {
        REPORT_FAIL("Reports overflow before overflowing");
        return false;
    }
    rcvb_push_message(&msg);
    if (!rcvb_has_overflowed()) {
        REPORT_FAIL("Doesn't report overflow after overflowing");
        return false;
    }
    rcvb_clear_overflow_flag();
    if (rcvb_has_overflowed()) {
        REPORT_FAIL("Reports overflow after clearing flag");
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
    if (!test_buffer_overflow()) {
        return false;
    }
    return true;
}
