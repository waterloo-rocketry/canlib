#include "can_buffer_layer.h"
#include "safe_ring_buffer.h"

static srb_ctx_t buf;
static cbl_ctx_t ctx;

void can_txb_init(void *pool, 
                size_t pool_size,
                bool (*is_transmit_buffer_empty)(void), 
                void (*can_send)(const can_msg_t *msg, uint8_t priority)) {
    ctx.is_transmit_buffer_empty = is_transmit_buffer_empty;
    ctx.can_send = can_send;
    srb_init(&buf, pool, pool_size, sizeof(can_msg_t));
}

bool can_txb_queue(const can_msg_t *msg, uint8_t priority) {
    // if the internal queue is out of room, returns false
    if (srb_is_full(&buf)) {
        return false;
    }
    // else, adds the message to the queue
    msg_t can_msg = {
        msg,
        priority
    };
    srb_push(&buf, &can_msg);
    return true;
}

void can_txb_check(void) {
    if ((*(ctx.is_transmit_buffer_empty))()) {
        msg_t can_msg;
        srb_pop(&buf, &can_msg);
        (*(ctx.can_send))(can_msg.msg, can_msg.priority);
    }
}