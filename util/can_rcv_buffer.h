#ifndef BUFFER_RECEIVED_CAN_MESSAGE_H_
#define BUFFER_RECEIVED_CAN_MESSAGE_H_

/*
 * A module for buffering CAN messages that are received from the CAN module.
 * The module operates as a ring buffer, with the memory provided by the caller
 * of the module.
 *
 * The whole goal of this module is allowing you to write application code that
 * doesn't have to concern itself with ISR vs main thread code. If you set
 * buffer_received_can_message as the can callback, then you can dequeue the
 * buffered messages from the main thread without worrying about missing any
 * messages from the bus.
 *
 * The ring buffer is designed to be pseudo-thread safe. None of the functions
 * are rentrant, and there are no locking or atomic mechanisms, but flags are
 * used to show whether a particular memory element contains valid data. This
 * should allow the writer function (buffer_received_can_message) and the reader
 * function (get_buffered_can_message) to operate from separate contexts (the
 * former can be run in the ISR and the latter in the main thread) without any
 * problems.
 */

#include "can.h"
#include <stdbool.h>
#include <stddef.h>

/*
 * Initializes the module. Pool is a memory buffer, which must be provided by
 * the caller. pool_size is the size of pool, in bytes (not in number of
 * can_t's)
 */
void rcvb_init(void *pool, size_t pool_size);

/*
 * Copies msg into our internal buffering system.
 *
 * This function fails silently if we're out of memory/space to hold the CAN
 * message. This is so that you can use this buffering system as the CAN
 * callback, whose signature is void
 */
void rcvb_push_message(const can_msg_t *msg);

/*
 * Returns true if the CAN receive buffer has ever overflowed.
 *
 * Because the push function is meant to be called from an ISR, it won't be
 * able to do anything if it runs out of memory, it will just drop the
 * message. That drop shouldn't be silent, so this function will return
 * true if a message has ever been dropped. You can clear this flag with
 * rcvb_reset_overflow_flag.
 */
bool rcvb_has_overflowed(void);

/*
 * Clears the overflow flag, so that rcvb_has_overflowed will start
 * returning false again. This function isn't perfectly concurrency safe:
 * If you call this function and during the call the receive buffer
 * overflows again, you'll miss that flag.
 */
void rcvb_clear_overflow_flag(void);

/*
 * Returns true if the receive buffer is full
 *
 * This function exists to make up for the signature of
 * rcvb_push_message. If this function returns true, then you know
 * that we're out of memory and we can't enqueue your new message
 */
bool rcvb_is_full(void);

/*
 * Returns false if there's a CAN message that has been buffered, but has not yet
 * been read. Returns true otherwise
 */
bool rcvb_is_empty(void);

/*
 * Gets the oldest buffered CAN message and puts it into msg, then dequeues
 * that message. Returns true if we were successfully able to grab a CAN message.
 */
bool rcvb_pop_message(can_msg_t *msg);

/*
 * Gets the oldest buffered CAN message and puts it into msg, and does not
 * dequeue it. Returns true if we were successfully able to grab a CAN message.
 */
bool rcvb_peek_message(can_msg_t *msg);

#endif
