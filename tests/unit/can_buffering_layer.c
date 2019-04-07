#include "can_buffering_layer.h"
#include "can_rcv_buffer.h"
#include "can_tx_buffer.h"
#include <stdio.h>

//if this test is running on hardware, don't actually print anything
#ifdef TARGET_LOCAL
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
    can_msg_t rcv;

    can_msg_t send;
    send.sid = 0x7ab,
    send.data_len = 4,

    send.data[0] = 0xab;
    send.data[1] = 0xcd;
    send.data[2] = 0xef;
    send.data[3] = 0xef;

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

    can_msg_t rcv;  // to be populated later
    can_msg_t send;

    send.sid = 0;
    send.data_len = 2;
    send.data[0] = 72;
    send.data[1] = 49;

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
    can_msg_t test;
    test.sid = 0x7FF;
    test.data_len = 8;
    for (int i = 0; i < 8; ++i) {
        test.data[i] = 0xff;
    }

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
    can_msg_t msg;
    msg.data_len = 0;
    msg.sid = 0x7FF;

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

bool test_can_buffering_layer_rx(void)
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

// Fake can transmit functions
static can_msg_t last_can_message;
static int can_msgs_sent = 0;
static bool tx_buffer_empty = true;
static void can_send(const can_msg_t *msg)
{
    last_can_message = *msg;
    can_msgs_sent++;
    tx_buffer_empty = false;
}
static bool can_tx_buffer_available(void)
{
    return tx_buffer_empty;
}

static bool test_send_single_message(void)
{
    tx_buffer_empty = true;
    can_msgs_sent = 0;
    uint8_t memory[100];
    txb_init((void *) memory,
             sizeof(memory),
             &can_send,
             &can_tx_buffer_available);

    can_msg_t send;
    send.sid = 0x7ab,
    send.data_len = 4,
    send.data[0] = 0xab;
    send.data[1] = 0xcd;
    send.data[2] = 0xef;
    send.data[3] = 0xef;

    if (can_msgs_sent != 0) {
        REPORT_FAIL("can_send called before any message queued");
        return false;
    }

    txb_enqueue(&send);

    /*
     * For now, do not send a CAN message during a call to txb_send, even if
     * the CAN buffer is available
     */
    if (can_msgs_sent != 0) {
        REPORT_FAIL("can_send called before heartbeat issued");
        return false;
    }

    txb_heartbeat();

    if (can_msgs_sent != 1 ||
        last_can_message.sid != 0x7ab ||
        last_can_message.data_len != 4 ||
        last_can_message.data[0] != 0xab ||
        last_can_message.data[1] != 0xcd ||
        last_can_message.data[2] != 0xef ||
        last_can_message.data[3] != 0xef) {
        REPORT_FAIL("can_send wasn't called after heartbeat");
        return false;
    }
    return true;
}

static bool test_send_ten_messages(void)
{
    tx_buffer_empty = true;
    can_msgs_sent = 0;
    uint8_t memory[14 * 10];
    txb_init((void *) memory,
             sizeof(memory),
             &can_send,
             &can_tx_buffer_available);

    uint8_t i;

    can_msg_t send;

    send.sid = 0;
    send.data_len = 2;
    send.data[0] = 72;
    send.data[1] = 49;

    for (i = 0; i < 10; ++i) {
        send.sid = i;
        txb_enqueue(&send);
    }
    for (i = 0; i < 10; ++i) {
        tx_buffer_empty = true;
        txb_heartbeat();
        send.sid = i;
        if (!can_msg_compare(&last_can_message, &send)) {
            REPORT_FAIL("it returned the wrong CAN value");
            return false;
        }
    }
    return true;
}

static bool test_can_buffer_full(void)
{
    tx_buffer_empty = false;
    can_msgs_sent = 0;
    uint8_t memory[100];
    txb_init((void *) memory,
             sizeof(memory),
             &can_send,
             &can_tx_buffer_available);

    can_msg_t send;
    send.sid = 0x7ab;
    send.data_len = 0;

    if (can_msgs_sent != 0) {
        REPORT_FAIL("can_send called before any message queued");
        return false;
    }

    txb_enqueue(&send);

    if (can_msgs_sent != 0) {
        REPORT_FAIL("can_send called before heartbeat issued");
        return false;
    }

    txb_heartbeat();

    if (can_msgs_sent != 0) {
        REPORT_FAIL("can_send called while buffer was full");
        return false;
    }

    tx_buffer_empty = true;
    txb_heartbeat();

    if (can_msgs_sent != 1 ||
        last_can_message.sid != 0x7ab ||
        last_can_message.data_len != 0) {
        REPORT_FAIL("can_send wasn't called after heartbeat");
        return false;
    }
    return true;
}

/*
 * If txb_heartbeat is called before any calls to txb_enqueue, can_send should
 * not be called. txb_heartbeat should only call can_send if the buffer is
 * empty and the message queue is not
 */
static bool test_can_heartbeat_without_enqueuing(void)
{
    tx_buffer_empty = true;
    can_msgs_sent = 0;
    uint8_t memory[100];
    txb_init((void *) memory,
             sizeof(memory),
             &can_send,
             &can_tx_buffer_available);

    if (can_msgs_sent != 0) {
        REPORT_FAIL("can_send called before any message queued");
        return false;
    }

    txb_heartbeat();

    if (can_msgs_sent != 0) {
        REPORT_FAIL("can_send called after heartbeat without any CAN message queued");
        return false;
    }

    return true;
}

bool test_can_buffering_layer_tx(void)
{
    if (!test_send_single_message()) {
        return false;
    }
    if (!test_send_ten_messages()) {
        return false;
    }
    if (!test_can_buffer_full()) {
        return false;
    }
    if (!test_can_heartbeat_without_enqueuing()) {
        return false;
    }
    return true;
}
