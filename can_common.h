#ifndef CAN_COMMON_H_
#define CAN_COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include "can.h"

/*
 * Creates a new CAN message. It is the caller's responsibility to
 * ensure that input_data contains the correct amount and order of
 * data to be sent. Function returns true if CAN message was
 * successfully built, else returns false. The only way for this
 * function to return false is if message_type is not one of the
 * message types defined in message_types.h, or input_data or output
 * are NULL pointers.
 */
bool build_can_message(uint16_t message_type,
                       uint32_t timestamp,
                       const uint8_t *input_data,
                       can_msg_t *output);


#endif // compile guard
