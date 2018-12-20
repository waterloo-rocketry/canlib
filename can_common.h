#ifndef CAN_COMMON_H_
#define CAN_COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include "can.h"

/*
 * Debug levels for the debugging messages (MSG_DEBUG_MSG). Lower
 * numbers are more serious debug things
 */
typedef enum {
    NONE  = 0,
    ERROR = 1,
    WARN  = 2,
    INFO  = 3,
    DEBUG = 4
} can_debug_level_t;

/*
 * Creates a new CAN message. It is the caller's responsibility to
 * ensure that input_data contains the correct amount and order of
 * data to be sent. Function returns true if CAN message was
 * successfully built, else returns false. The only way for this
 * function to return false is if message_type is not one of the
 * message types defined in message_types.h, or input_data or output
 * are NULL pointers.
 *
 * Try to avoid calling this function from application code if you
 * can, TODO write out some wrapper functions
 */
bool build_can_message(uint16_t message_type,
                       uint32_t timestamp,
                       const uint8_t *input_data,
                       can_msg_t *output);

/*
 * Strips the board unique ID from msg, and returns the SID. Contains
 * no error checking, so if you pass an illegal SID, you could get
 * back a nonsensical value.
 */
uint16_t get_message_type(const can_msg_t *msg);

/*
 * Returns true if msg is of type SENSOR_ACC, SENSOR_GYRO, SENSOR_MAG,
 * or SENSOR_ANALOG. Otherwise it returns false
 */
bool is_sensor_data(const can_msg_t *msg);

/*
 * If MSG is a DEBUG_MSG message, return its debug level, else return
 * NONE
 */
can_debug_level_t debug_level_message(const can_msg_t *msg);


#endif // compile guard
