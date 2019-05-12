#ifndef DSPIC33EPXXXGP50X_CAN_H_
#define DSPIC33EPXXXGP50X_CAN_H_


#include "../can.h"

/*
 * Initialize the CAN driver on a dsPIC33whatever. Note that this
 * function DOES NOT setup the inputs and outputs from the CAN module
 * to the output pins, application code must do that. In order to do
 * that, CANRXPPS must be set to the proper pin value for the CANRX
 * pin, and ___PPS must be set to 0x33 to mark it as outputting from
 * the CAN module. In addition, TRIS and ANSEL registers for whatever
 * pin is being used must be set to the right values.
 */
void init_can(const can_timing_t *timing,
              void (*receive_callback)(const can_msg_t *message),
              bool run_in_loopback);

// send a CAN message. Priority is 3 or less, higher means higher
// priority
void can_send(const can_msg_t* message);

// returns true if the CAN module is ready to send a message
bool can_send_rdy(void);

#endif // compile guard
