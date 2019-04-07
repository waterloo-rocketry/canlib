#include "can_tx_buffer.h"
#include "safe_ring_buffer.h"
#include <stdio.h>

typedef struct {
    void (*can_send_fp)(const can_msg_t *);
    bool (*can_tx_ready)(void);
} cbl_ctx_t;

static srb_ctx_t buf;
static cbl_ctx_t ctx;
can_msg_t msg_pt2;

void txb_init(void *pool, size_t pool_size,
              void (*can_send_fp)(const can_msg_t *),
              bool (*can_tx_ready)(void)) {
    ctx.can_send_fp = can_send_fp;
    ctx.can_tx_ready = can_tx_ready;
    srb_init(&buf, pool, pool_size, sizeof(can_msg_t));
}

void txb_enqueue(const can_msg_t *msg) {
    if (!srb_is_full(&buf)) {
        srb_push(&buf, msg);
    }
}

void txb_heartbeat(void) {
    if ((*(ctx.can_tx_ready))()) {
        srb_pop(&buf, &msg_pt2);
        (*(ctx.can_send_fp))(&msg_pt2);
    }
}