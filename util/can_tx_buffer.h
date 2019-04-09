#include <stdbool.h>
#include <stddef.h>
#include "can.h"

/*
 * Initialization function. Sets up memory for the buffer, and caches the
 * function pointers
 *
 * can_tx_ready is a function that returns true if we can currently send a can
 * message, otherwise it returns false
 */
void txb_init(void *pool, size_t pool_size,
              void (*can_send_fp)(const can_msg_t *),
              bool (*can_tx_ready)(void));

/*
 * Buffers msg. If there is room in the can_tx buffer, this function will not
 * send the message. The only calls to can_send_fp will be made during
 * txb_heartbeat
 */
bool txb_enqueue(const can_msg_t *msg);

/*
 * This function is called every iteration through the main application loop.
 * If there are any messages that are in the "to transmit" queue, and
 * can_tx_ready() returns true, then can_send will be called with that message
 */
void txb_heartbeat(void);
