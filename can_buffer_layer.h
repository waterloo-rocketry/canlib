#include <stdbool.h>
#include "can.h"

/*
* header file for a buffering layer for messages being sent to can_send
*/

void can_txb_init(void *pool, 
                size_t pool_size,
                bool (*is_transmit_buffer_empty)(void), 
                void (*can_send)(const can_msg_t *msg, uint8_t priority));

// returns false if the internal queue is out of room
bool can_txb_queue(const can_msg_t *msg, uint8_t priority);

void can_txb_something(void);

typedef struct {
    bool (*is_transmit_buffer_empty)(void);
    void (*can_send)(const can_msg_t *msg, uint8_t priority);
} cbl_ctx_t;

typedef struct {
    can_msg_t *msg;
    uint8_t priority;
} msg_t;