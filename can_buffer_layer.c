#include "can_buffer_layer.h"

void can_txb_init(bool (*is_transmit_buffer_empty)(void), void (*can_send)(const can_msg_t *msg, uint8_t priority));

// returns false if the internal queue is out of room
bool can_txb_queue(const can_msg_t *msg);

void can_txb_something(void);

bool is_transmit_buffer_empty() {
    if (TXB0CONbits.TXREQ = 0) {
        return true;
    }
    return false;
}