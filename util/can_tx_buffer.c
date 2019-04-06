#include "can_tx_buffer.h"

void txb_init(void *pool, size_t pool_size,
              void (*can_send_fp)(const can_msg_t *),
              bool (*can_tx_ready)(void))
{

}

void txb_send(const can_msg_t *msg)
{

}

void txb_heartbeat(void)
{

}
