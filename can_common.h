#ifndef CAN_COMMON_H_
#define CAN_COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include "can.h"
#include "message_types.h"

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
 * This macro creates a new debug message, and stores it in
 * debug_macro_output. The reason that this is a macro and not a
 * function is that debug messages have the line number at which they
 * are created embedded in their data. This is so that we can review
 * the code later to see where the debug was issued from, and
 * hopefully find the cause of the problem
 */
#define DEBUG(debug_macro_level, debug_macro_timestamp, debug_macro_output) \
    do {                                                                \
        uint8_t debug_macro_data[5] = {(debug_macro_level << 4) | ((__LINE__ >> 8) & 0xF), \
                                       __LINE__ & 0xFF,                 \
                                       0,0,0};                          \
        build_debug_msg( debug_macro_timestamp,                         \
                         debug_macro_data,                              \
                         &debug_macro_output);                          \
    } while(0)

//***************************************************************************//
//                         Message Type Functions                            //
//***************************************************************************//
/*
* These functions are meant to format CAN messages of each message type
* properly. They contain basic error checking to make sure that the message
* type is valid and that all pointers passed in are valid.
*
* For the internal message format, refer to message_types.h. If you add a
* new message type, please create a new function here. The internals of the
* message should not be used by application code directly.

* Used for general system-wide commands. Command types are defined in
* message_types.h
*/
bool build_general_cmd_msg(uint32_t timestamp,
                           enum GEN_CMD cmd,
                           can_msg_t *output);

bool build_debug_msg(uint32_t timestamp,
                     uint8_t *debug_data,
                     can_msg_t *output);

bool build_debug_printf(uint8_t *data,
                        can_msg_t *output);

/*
 * Used to send injector and vent commands
 */
bool build_valve_cmd_msg(uint32_t timestamp,
                         enum VALVE_STATE valve_cmd,
                         uint16_t message_type,  // vent or injector
                         can_msg_t *output);

/*
* Used to send injector/vent status: current and desired
*/
bool build_valve_stat_msg(uint32_t timestamp,
                          enum VALVE_STATE valve_state,
                          enum VALVE_STATE req_valve_state,
                          uint16_t vent_batt_voltage_mv,
                          uint16_t inj_batt_voltage_mv,
                          uint16_t message_type,    // vent or injector
                          can_msg_t *output);

/*
* Used by each board to send status messages. Error codes and their
* corresponding supplemental data are defined in message_types.h.
* This function may need to be modified to better hide the internals.
*/
bool build_board_stat_msg(uint32_t timestamp,
                          enum BOARD_STATUS error_code,
                          uint8_t *error_data,
                          uint8_t error_data_len,
                          can_msg_t *output);

/*
* Used to send 16-bit IMU data values. It is assumed that an array
* of 3 values is sent (X, Y, and Z axes).
*/
bool build_imu_data_msg(uint16_t message_type,  // acc, gyro, mag
                        uint32_t timestamp,
                        uint16_t *imu_data,     // x, y, z
                        can_msg_t *output);

/*
* Used to send analog sensor data. The units of the sensor data are
* not nailed down at this point and will likely differ based on the
* sensor id.
*/
bool build_analog_data_msg(uint32_t timestamp,
                           enum SENSOR_ID sensor_id,
                           uint16_t sensor_data,
                           can_msg_t *output);

/*
 * Gets the general command contained in a general command message.
 * Returns -1 if the provided message is not a general cmd message.
 */
int get_general_cmd_type(const can_msg_t *msg);

 /*
 * Returns the current valve state based on limit switch readings.
 * Returns -1 if the provided message is not a vent/injector status message.
 */
int get_curr_valve_state(const can_msg_t *msg);

 /*
* Returns the requested valve state from a valve command or
* status message. Returns -1 if the provided message is not
* a valve cmd/status.
*/
int get_req_valve_state(const can_msg_t *msg);

/*
 * Strips the board unique ID from msg, and returns the SID. Contains
 * no error checking, so if you pass an illegal SID, you could get
 * back a nonsensical value.
 */
uint16_t get_message_type(const can_msg_t *msg);

/*
 * Strips the message type from msg, and returns the SID. Basically
 * the opposite of get_message_type
 */
uint8_t get_board_unique_id(const can_msg_t *msg);

/*
 * Returns the timestamp of the message. This can be either two or
 * three bytes depending on the message type.
 */
uint32_t get_timestamp(const can_msg_t *msg);

/*
 * Returns true if msg is of type SENSOR_ACC, SENSOR_GYRO, SENSOR_MAG,
 * or SENSOR_ANALOG. Otherwise it returns false
 */
bool is_sensor_data(const can_msg_t *msg);

/*
 * Gets IMU data from an IMU message. Returns true if
 * successful, false if the input is invalid.
 */
bool get_imu_data(const can_msg_t *msg,
                  uint16_t *output_x,
                  uint16_t *output_y,
                  uint16_t *output_z);

/*
 * Gets analog data (the sensor ID and data itself) from an
 * analog message. Returns true if successful, false if
 * the input is invalid.
 */
bool get_analog_data(const can_msg_t *msg,
                     enum SENSOR_ID *sensor_id,
                     uint16_t *output_data);

/*
 * If MSG is a DEBUG_MSG message, return its debug level, else return
 * NONE
 */
can_debug_level_t message_debug_level(const can_msg_t *msg);

/*
 * These commands build CAN messages whose data bytes are the ASCII characters
 * from string. The functions return a pointer to the first character in string
 * which wasn't copied into output.
 *
 * So if you pass the function string="a long string", "a long s" will be put into
 * output, and "tring" will be returned. You can tell if all of the string was
 * copied into output if the return value points to a '\0'
 *
 * You can build and send an arbitrary length string with the following C code:
 *   const char* string = "arbitrarily long string";
 *   can_msg_t output;
 *   while (*string) {
 *       string = build_printf_can_message(string, &output);
 *       can_send(&output);
 *   }
 */
const char *build_printf_can_message(const char *string, can_msg_t *output);
const char *build_radio_cmd_can_message(const char *string, can_msg_t *output);

#endif // compile guard
