#ifndef PIC18F26K83_CAN_H_
#define PIC18F26K83_CAN_H_

#include "../can.h"
#include <stdbool.h>

/*
 * Initialize the CAN driver on a PI18fC26fk83. Note that this function
 * DOES NOT setup the inputs and outputs from the CAN module to the
 * output pins, application code must do that. In order to do that,
 * CANRXPPS must be set to the proper pin value for the CANRX pin, and
 * ___PPS must be set to 0x33 to mark it as outputting from the CAN
 * module. In addition, TRIS and ANSEL registers for whatever pin
 * is being used must be set to the right values.
 */
void can_init(const can_timing_t *timing,
              void (*receive_callback)(const can_msg_t *message));

// send a CAN message
void can_send(const can_msg_t* message);

// returns true if the CAN module is ready to send a message
bool can_send_rdy(void);

// call this function in an interrupt context whenever PIR5 is nonzero
void can_handle_interrupt(void);

#endif // compile guard
