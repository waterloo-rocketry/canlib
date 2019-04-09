#include "can_tx_buffer.h"
#include "safe_ring_buffer.h"

typedef struct {
    void (*can_send_fp)(const can_msg_t *);
    bool (*can_tx_ready)(void);
} cbl_ctx_t;

// context to store info for the safe ring buffer
static srb_ctx_t buf;

// context to store pointers to can_send_fp and can_tx_ready
static cbl_ctx_t ctx;

void txb_init(void *pool, size_t pool_size,
              void (*can_send_fp)(const can_msg_t *),
              bool (*can_tx_ready)(void)) {
    ctx.can_send_fp = can_send_fp;
    ctx.can_tx_ready = can_tx_ready;
    srb_init(&buf, pool, pool_size, sizeof(can_msg_t));
}

bool txb_enqueue(const can_msg_t *msg) {
    if (srb_is_full(&buf)) {
        return false;
    }
    srb_push(&buf, msg);
    return true;
}

void txb_heartbeat(void) {
    if (!srb_is_empty(&buf)) {
        if ((*(ctx.can_tx_ready))()) {
            can_msg_t msg_sent;
            srb_pop(&buf, &msg_sent);
            (*(ctx.can_send_fp))(&msg_sent);
        }
    }
}