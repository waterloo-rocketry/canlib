#include "can_common.h"
#include "message_types.h"
#include <stddef.h>
#include <assert.h>

// this symbol should be defined in the project's Makefile, but if it
// isn't, issue a warning and set it to 0
#ifndef  BOARD_UNIQUE_ID
#warning BOARD_UNIQUE_ID not defined, please set that up in project
#define  BOARD_UNIQUE_ID 0
#endif

// Helper function for populating CAN messages
static void write_timestamp_uint16_t(uint16_t timestamp, can_msg_t *output)
{
    output->data[0] = (timestamp >> 8) & 0xff;
    output->data[1] = (timestamp >> 0) & 0xff;
}

static void write_timestamp_uint24_t(uint32_t timestamp, can_msg_t *output)
{
    output->data[0] = (timestamp >> 16) & 0xff;
    output->data[1] = (timestamp >> 8) & 0xff;
    output->data[2] = (timestamp >> 0) & 0xff;
}

void build_general_cmd_msg(uint32_t timestamp,
                           uint8_t cmd,
                           can_msg_t *output)
{
    assert(output != NULL);

    output->sid = MSG_GENERAL_CMD | BOARD_UNIQUE_ID;
    write_timestamp_uint24_t(timestamp, output);
    output->data[3] = cmd;
    output->data_len = 4;   // 3 bytes timestamp, 1 byte data
}

void build_debug_msg(uint32_t timestamp,
                     uint8_t *debug_data,
                     can_msg_t *output)
{
    assert(output != NULL);
    assert(debug_data != NULL);

    output->sid = MSG_DEBUG_MSG | BOARD_UNIQUE_ID;
    write_timestamp_uint24_t(timestamp, output);

    output->data[3] = debug_data[0];
    output->data[4] = debug_data[1];
    output->data[5] = debug_data[2];
    output->data[6] = debug_data[3];
    output->data[7] = debug_data[4];

    output->data_len = 8;
}

void build_debug_printf(uint8_t *input_data,
                        can_msg_t *output)
{
    assert(input_data != NULL);
    assert(output != NULL);

    output->sid = MSG_DEBUG_PRINTF | BOARD_UNIQUE_ID;
    for (int i = 0; i < 8; ++i) {
        output->data[i] = input_data[i];
    }
    output->data_len = 8;
}

void build_valve_cmd_msg(uint32_t timestamp,
                         uint8_t valve_cmd,
                         uint16_t message_type,  // vent or injector
                         can_msg_t *output)
{
    assert(message_type == MSG_VENT_VALVE_CMD
           || message_type == MSG_INJ_VALVE_CMD);
    assert(output != NULL);

    output->sid = message_type | BOARD_UNIQUE_ID;
    write_timestamp_uint24_t(timestamp, output);

    output->data[3] = valve_cmd;
    output->data_len = 4;   // 3 bytes timestamp, 1 byte data
}

void build_valve_stat_msg(uint32_t timestamp,
                          uint8_t valve_state,
                          uint8_t req_valve_state,
                          uint16_t message_type,    // vent or injector
                          can_msg_t *output)
{
    assert(message_type == MSG_VENT_VALVE_STATUS
           || message_type == MSG_INJ_VALVE_STATUS);
    assert(output != NULL);

    output->sid = message_type | BOARD_UNIQUE_ID;
    write_timestamp_uint24_t(timestamp, output);

    output->data[3] = valve_state;
    output->data[4] = req_valve_state;
    output->data_len = 4;   // 3 bytes timestamp, 2 bytes data
}

// This might need to be made more granular - it doesn't quite hide
// the data layout properly.
void build_board_stat_msg(uint32_t timestamp,
                          uint8_t error_code,
                          uint8_t *error_data,
                          uint8_t error_data_len,
                          can_msg_t *output)
{
    assert(error_data != NULL);
    assert(output != NULL);
    assert(error_data_len <= 4);

    output->sid = MSG_GENERAL_BOARD_STATUS | BOARD_UNIQUE_ID;
    write_timestamp_uint24_t(timestamp, output);

    output->data[3] = error_code;
    for (int i = 0; i < error_data_len; ++i) {
        // error data goes in message bytes 4-7
        output->data[4 + i] = error_data[i];
    }

    // ugly but: 3 bytes timestamp, 1 byte error code, x bytes data
    output->data_len = 4 + error_data_len;
}

void build_imu_data_msg(uint16_t message_type,
                        uint16_t timestamp,   // acc, gyro, mag
                        uint16_t *imu_data,   // x, y, z
                        can_msg_t *output)
{
    assert(message_type == MSG_SENSOR_ACC
           || message_type == MSG_SENSOR_GYRO
           || message_type == MSG_SENSOR_MAG);
    assert(imu_data != NULL);
    assert(output != NULL);

    output->sid = message_type | BOARD_UNIQUE_ID;
    write_timestamp_uint16_t(timestamp, output);

    // X value
    output->data[2] = (imu_data[0] >> 8) & 0xff;
    output->data[3] = (imu_data[0] >> 0) & 0xff;

    // Y value
    output->data[4] = (imu_data[1] >> 8) & 0xff;
    output->data[5] = (imu_data[1] >> 0) & 0xff;

    // Z value
    output->data[6] = (imu_data[2] >> 8) & 0xff;
    output->data[7] = (imu_data[2] >> 0) & 0xff;

    // this message type uses the entire data field
    output->data_len = 8;
}

void build_analog_data_msg(uint16_t timestamp,
                           uint8_t sensor_id,
                           uint16_t sensor_data,
                           can_msg_t *output)
{
    assert(output != NULL);

    output->sid = MSG_SENSOR_ANALOG | BOARD_UNIQUE_ID;
    write_timestamp_uint16_t(timestamp, output);

    output->data[2] = sensor_id;
    output->data[3] = (sensor_data >> 8) & 0xff;
    output->data[4] = (sensor_data >> 0) & 0xff;

    output->data_len = 5;
}

int get_curr_valve_state(can_msg_t *msg)
{
    assert(msg != NULL);

    uint16_t msg_type = get_message_type(msg);
    if (msg_type == MSG_VENT_VALVE_STATUS || msg_type == MSG_INJ_VALVE_STATUS) {
        return msg->data[3];
    } else {
        // not a valid field for this message type
        return -1;
    }
}

int get_req_valve_state(can_msg_t *msg)
{
    assert(msg != NULL);

    uint16_t msg_type = get_message_type(msg);
    switch (msg_type) {
        case MSG_VENT_VALVE_STATUS:
        case MSG_INJ_VALVE_STATUS:
            return msg->data[4];

        case MSG_INJ_VALVE_CMD:
        case MSG_VENT_VALVE_CMD:
            return msg->data[3];

        default:
            // not a valid field for this message type
            return -1;
    }
}

uint16_t get_message_type(const can_msg_t *msg)
{
    assert(msg != NULL);
    return (msg->sid & 0x7E0);
}

uint8_t get_board_unique_id(const can_msg_t *msg)
{
    assert(msg != NULL);
    return ((uint8_t) (msg->sid & 0x1F));
}

bool is_sensor_data(const can_msg_t *msg)
{
    assert(msg != NULL);

    uint16_t type = get_message_type(msg);
    if (type == MSG_SENSOR_ACC ||
        type == MSG_SENSOR_GYRO ||
        type == MSG_SENSOR_MAG ||
        type == MSG_SENSOR_ANALOG) {
        return true;
    } else {
        return false;
    }
}

can_debug_level_t message_debug_level(const can_msg_t *msg)
{
    assert(msg != NULL);

    uint16_t type = get_message_type(msg);
    if (type != MSG_DEBUG_MSG) {
        return NONE;
    } else {
        // As per the spreadsheet, the debug level of a DEBUG_MSG is
        // stored in the top nibble of the 4th data byte (yeah, I
        // know, I'm sorry). To get at it, you right shift the 4th
        // data byte by 4. bitwise anding with 0xf probably isn't
        // necessary, but it ensures that the returned data is no more
        // than 4 bits.
        return ((msg->data[3] >> 4) & 0xf);
    }
}

/* In the grand tradition of C programmers, string handling funcitons
 * must be as convoluted as possible.  If you call this function with
 * a string that is longer than 8 bytes, it's impossible to fit all of
 * that into one CAN message. So what this function does is it puts
 * the first 8 bytes of the string into the CAN message output, then
 * returns a pointer that's 8 bytes ahead of the string (string +
 * 8). If it's called with less than 8 characters of data, it returns
 * a pointer to '\0' (the null terminator in string). This lets you
 * write code like the following:
 *
 * const char *string = "a really long message to be sent on CAN";
 * can_msg_t to_send;
 * while (*string) {
 *     string = build_printf_can_message(string, &to_send);
 *     can_send(&to_send);
 * }
 */
const char *build_printf_can_message(const char *string, can_msg_t *output)
{
    assert(string != NULL);
    assert(output != NULL);

    // set the SID of ouput
    output->sid = (MSG_DEBUG_PRINTF | BOARD_UNIQUE_ID);
    uint8_t i;
    for (i = 0; i < 8; ++i) {
        if (*string == '\0') {
            output->data_len = i;
            return string;
        }
        output->data[i] = *string;
        string++;
    }
    output->data_len = i;
    return string;
}
