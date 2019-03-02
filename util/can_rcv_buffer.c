#include "can_rcv_buffer.h"
#include <string.h>

typedef struct {
    can_msg_t message;
    //if valid, we can read this
    bool vld;
} rb_element;

static rb_element *memory_pool;
static uint8_t read_index;
static uint8_t write_index;
static uint8_t number_of_elements;
static uint8_t element_size;
static bool overflow_flag;

void rcvb_init(void *pool, size_t pool_size)
{
    element_size = (uint8_t) sizeof(rb_element);
    number_of_elements = pool_size / element_size;
    read_index = write_index = 0;
    memory_pool = (rb_element *) pool;
    overflow_flag = false;

    //mark all elements as invalid
    rb_element *iter = memory_pool;
    uint8_t i;
    for (i = 0; i < number_of_elements; ++i) {
        iter->vld = false;
        iter++;
    }
}

void rcvb_push_message(const can_msg_t *msg)
{
    //check that write index points at empty element
    if (rcvb_is_full()) {
        overflow_flag = true;
        return;
    }

    rb_element *wr = &(memory_pool[write_index]);
    memcpy(&(wr->message), msg, sizeof(can_msg_t));
    wr->vld = true;
    write_index++;
    if (write_index >= number_of_elements) {
        write_index = 0;
    }
}

bool rcvb_has_overflowed(void)
{
    return overflow_flag;
}

void rcvb_clear_overflow_flag(void)
{
    overflow_flag = false;
}

bool rcvb_is_full(void)
{
    //we're out of memory if write_index has valid set (this means that we've
    //reached where we're supposed to be reading from)
    if (number_of_elements == 0) {
        return false;
    }
    rb_element *check = &(memory_pool[write_index]);
    return check->vld;
}

bool rcvb_is_empty(void)
{
    rb_element *rd = &(memory_pool[read_index]);
    return !rd->vld;
}

bool rcvb_pop_message(can_msg_t *msg)
{
    if (!rcvb_peek_message(msg)) {
        return false;
    }
    rb_element *rd = &(memory_pool[read_index]);
    rd->vld = false;

    read_index++;
    if (read_index >= number_of_elements) {
        read_index = 0;
    }
    return true;
}

bool rcvb_peek_message(can_msg_t *msg)
{
    rb_element *rd = &(memory_pool[read_index]);
    if (!rd->vld) {
        return false;
    }
    memcpy(msg, &(rd->message), sizeof(can_msg_t));
    return true;
}
