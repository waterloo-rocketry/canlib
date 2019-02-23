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
void receive_buffer_init(void *pool, size_t pool_size);

/*
 * Copies msg into our internal buffering system.
 *
 * This function fails silently if we're out of memory/space to hold the CAN
 * message. This is so that you can use this buffering system as the CAN
 * callback, whose signature is void
 */
void buffer_received_can_message(const can_msg_t *msg);

/*
 * Returns true if there's room available to buffer another CAN message
 *
 * This function exists to make up for the signature of
 * buffer_received_can_message. If this function returns false, then you know
 * that we're out of memory and we can't enqueue your new message
 */
bool available_received_can_message_space(void);

/*
 * Returns true if there's a CAN message that has been buffered, but has not yet
 * been read. Returns false otherwise
 */
bool buffered_received_can_message_available(void);

/*
 * Gets the oldest buffered CAN message and puts it into msg, then dequeues
 * that message. Returns true if we were successfully able to grab a CAN message.
 */
bool get_buffered_can_message(can_msg_t *msg);

/*
 * Gets the oldest buffered CAN message and puts it into msg, and does not
 * dequeue it. Returns true if we were successfully able to grab a CAN message.
 */
bool peek_buffered_can_message(can_msg_t *msg);

#endif
