#include "can_rcv_buffer.h"
#include "safe_ring_buffer.h"

static srb_ctx_t buf;
static bool overflow_flag;

void rcvb_init(void *pool, size_t pool_size)
{
    srb_init(&buf, pool, pool_size, sizeof(can_msg_t));
    overflow_flag = false;
}

void rcvb_push_message(const can_msg_t *msg)
{
    if (!srb_push(&buf, (void *) msg)) {
        overflow_flag = true;
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
    return srb_is_full(&buf);
}

bool rcvb_is_empty(void)
{
    return srb_is_empty(&buf);
}

bool rcvb_pop_message(can_msg_t *msg)
{
    return srb_pop(&buf, (void *) msg);
}

bool rcvb_peek_message(can_msg_t *msg)
{
    return srb_peek(&buf, (void *) msg);
}
