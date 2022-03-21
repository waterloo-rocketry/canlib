#include "can_common.h"
#include "message_types.h"
#include <stddef.h>

// this symbol should be defined in the project's Makefile, but if it
// isn't, issue a warning and set it to 0
#ifndef  BOARD_UNIQUE_ID
#warning BOARD_UNIQUE_ID not defined, please set that up in project
#define  BOARD_UNIQUE_ID 0
#endif

// Helper function for populating CAN messages
static void write_timestamp_2bytes(uint16_t timestamp, can_msg_t *output)
{
    output->data[0] = (timestamp >> 8) & 0xff;
    output->data[1] = (timestamp >> 0) & 0xff;
}

static void write_timestamp_3bytes(uint32_t timestamp, can_msg_t *output)
{
    output->data[0] = (timestamp >> 16) & 0xff;
    output->data[1] = (timestamp >> 8) & 0xff;
    output->data[2] = (timestamp >> 0) & 0xff;
}

bool build_general_cmd_msg(uint32_t timestamp,
                           enum GEN_CMD cmd,
                           can_msg_t *output)
{
    if (!output) { return false; }

    output->sid = MSG_GENERAL_CMD | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);
    output->data[3] = (uint8_t) cmd;
    output->data_len = 4;   // 3 bytes timestamp, 1 byte data

    return true;
}

bool build_debug_msg(uint32_t timestamp,
                     uint8_t *debug_data,
                     can_msg_t *output)
{
    if (!output) { return false; }
    if (!debug_data) { return false; }

    output->sid = MSG_DEBUG_MSG | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = debug_data[0];
    output->data[4] = debug_data[1];
    output->data[5] = debug_data[2];
    output->data[6] = debug_data[3];
    output->data[7] = debug_data[4];

    output->data_len = 8;
    return true;
}

bool build_debug_printf(uint8_t *input_data,
                        can_msg_t *output)
{
    if (!output) { return false; }
    if (!input_data) { return false; }

    output->sid = MSG_DEBUG_PRINTF | BOARD_UNIQUE_ID;
    for (int i = 0; i < 8; ++i) {
        output->data[i] = input_data[i];
    }
    output->data_len = 8;
    return true;
}

bool build_reset_msg(uint32_t timestamp,
                     uint8_t board_id,
                     can_msg_t *output)
{
    if (!output) { return false; }

    output->sid = MSG_RESET_CMD | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);
    output->data[3] = board_id;
    output->data_len = 4; // 3 bytes timestamp, 1 byte data

    return true;
}

bool build_valve_cmd_msg(uint32_t timestamp,
                         enum VALVE_STATE valve_cmd,
                         uint16_t message_type,  // vent or injector
                         can_msg_t *output)
{
    if (!output) { return false; }
    if (!(message_type == MSG_VENT_VALVE_CMD
           || message_type == MSG_INJ_VALVE_CMD)) {
        return false;
    }

    output->sid = message_type | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = (uint8_t) valve_cmd;
    output->data_len = 4;   // 3 bytes timestamp, 1 byte data

    return true;
}

bool build_valve_stat_msg(uint32_t timestamp,
                          enum VALVE_STATE valve_state,
                          enum VALVE_STATE req_valve_state,
                          uint16_t message_type,    // vent or injector
                          can_msg_t *output)
{
    if (!output) { return false; }
    if (!(message_type == MSG_VENT_VALVE_STATUS
           || message_type == MSG_INJ_VALVE_STATUS)) {
        return false;
    }

    output->sid = message_type | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = (uint8_t) valve_state;
    output->data[4] = (uint8_t) req_valve_state;
    output->data_len = 5;   // 3 bytes timestamp, 2 bytes data

    return true;
}

bool build_arm_cmd_msg(uint32_t timestamp,
                       uint8_t alt_num,
                       enum ARM_STATE arm_cmd,
                       can_msg_t *output)
{
    if (!output) { return false; }

    output->sid = MSG_ALT_ARM_CMD | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    // 4 msb are used for arm state, 4 lsb used for altimeter number
    output->data[3] = (arm_cmd << 4) | (alt_num & 0x0F);
    output->data_len = 4; // 3 bytes timestamp, 1 byte arm state + alt num

    return true;
}

bool build_arm_stat_msg(uint32_t timestamp,
                        uint8_t alt_num,
                        enum ARM_STATE arm_state,
                        uint16_t v_drogue,
                        uint16_t v_main,
                        can_msg_t *output)
{
    if (!output) { return false; }

    output->sid = MSG_ALT_ARM_STATUS | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    // 4 msb are used for arm state, 4 lsb used for altimeter number
    output->data[3] = (arm_state << 4) | (alt_num & 0x0F);
    // drogue voltage
    output->data[4] = v_drogue >> 8;     // 8 msb
    output->data[5] = v_drogue & 0x00FF; // 8 lsb
    // main voltage
    output->data[6] = v_main >> 8;     // 8 msb
    output->data[7] = v_main & 0x00FF; // 8 lsb
    // 3 bytes timestamp, 1 byte arm state + alt num, 4 bytes voltages
    output->data_len = 8;

    return true;
}

// This might need to be made more granular - it doesn't quite hide
// the data layout properly.
bool build_board_stat_msg(uint32_t timestamp,
                          enum BOARD_STATUS error_code,
                          uint8_t *error_data,
                          uint8_t error_data_len,
                          can_msg_t *output)
{
    if (error_data_len > 0 && !error_data) { return false; }
    if (!output) { return false; }
    if (error_data_len > 4) { return false; }

    output->sid = MSG_GENERAL_BOARD_STATUS | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = (uint8_t) error_code;
    for (int i = 0; i < error_data_len; ++i) {
        // error data goes in message bytes 4-7
        output->data[4 + i] = error_data[i];
    }

    // ugly but: 3 bytes timestamp, 1 byte error code, x bytes data
    output->data_len = 4 + error_data_len;

    return true;
}

bool build_imu_data_msg(uint16_t message_type,
                        uint32_t timestamp,   // acc, gyro, mag
                        uint16_t *imu_data,   // x, y, z
                        can_msg_t *output)
{
    if (!output) { return false; }
    if (!imu_data) { return false; }
    if (!(message_type == MSG_SENSOR_ACC
           || message_type == MSG_SENSOR_GYRO
           || message_type == MSG_SENSOR_MAG)) {
        return false;
    }

    output->sid = message_type | BOARD_UNIQUE_ID;
    write_timestamp_2bytes(timestamp, output);

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

    return true;
}

bool build_analog_data_msg(uint32_t timestamp,
                           enum SENSOR_ID sensor_id,
                           uint16_t sensor_data,
                           can_msg_t *output)
{
    if (!output) { return false; }

    output->sid = MSG_SENSOR_ANALOG | BOARD_UNIQUE_ID;
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = (uint8_t) sensor_id;
    output->data[3] = (sensor_data >> 8) & 0xff;
    output->data[4] = (sensor_data >> 0) & 0xff;

    output->data_len = 5;

    return true;
}

bool build_altitude_data_msg(uint32_t timestamp,
                             int32_t altitude,
                             can_msg_t *output)
{
    if(!output) { return false; }

    output->sid =  MSG_SENSOR_ALTITUDE | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = (altitude >> 24) & 0xFF;
    output->data[4] = (altitude >> 16) & 0xFF;
    output->data[5] = (altitude >> 8) & 0xFF;
    output->data[6] = altitude & 0xFF;
    output->data_len = 7;

    return true;
}

bool build_gps_time_msg(uint32_t timestamp,
                        uint8_t utc_hours,
                        uint8_t utc_mins,
                        uint8_t utc_secs,
                        uint8_t utc_dsecs,
                        can_msg_t *output)
{
    if (!output) { return false; }

    output->sid = MSG_GPS_TIMESTAMP | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = utc_hours;
    output->data[4] = utc_mins;
    output->data[5] = utc_secs;
    output->data[6] = utc_dsecs;

    output->data_len = 7;

    return true;
}

bool build_gps_lat_msg(uint32_t timestamp,
                       uint8_t degrees,
                       uint8_t minutes,
                       uint16_t dminutes,
                       uint8_t direction,
                       can_msg_t *output)
{
    if (!output) { return false; }

    output->sid = MSG_GPS_LATITUDE | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = degrees;
    output->data[4] = minutes;
    output->data[5] = dminutes >> 8;
    output->data[6] = dminutes & 0xFF;
    output->data[7] = direction;

    output->data_len = 8;

    return true;
}

bool build_gps_lon_msg(uint32_t timestamp,
                       uint8_t degrees,
                       uint8_t minutes,
                       uint16_t dminutes,
                       uint8_t direction,
                       can_msg_t *output)
{
    if (!output) { return false; }

    output->sid = MSG_GPS_LONGITUDE | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = degrees;
    output->data[4] = minutes;
    output->data[5] = dminutes >> 8;
    output->data[6] = dminutes & 0xFF;
    output->data[7] = direction;

    output->data_len = 8;

    return true;
}

bool build_gps_alt_msg(uint32_t timestamp,
                       uint16_t altitude,
                       uint8_t daltitude,
                       uint8_t units,
                       can_msg_t *output)
{
    if (!output) { return false; }

    output->sid = MSG_GPS_ALTITUDE | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = (altitude >> 8) & 0xff;
    output->data[4] = (altitude >> 0) & 0xff;
    output->data[5] = daltitude;
    output->data[6] = units;

    output->data_len = 7;

    return true;
}

bool build_gps_info_msg(uint32_t timestamp,
                       uint8_t num_sat,
                       uint8_t quality,
                       can_msg_t *output)
{
    if (!output) { return false; }

    output->sid = MSG_GPS_INFO | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = num_sat;
    output->data[4] = quality;

    output->data_len = 5;

    return true;
}

bool build_fill_msg(uint32_t timestamp,
                           uint8_t lvl,
                           uint8_t direction,
                           can_msg_t *output)
{
    if (!output) { return false; }

    output->sid = MSG_FILL_LVL | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = lvl;
    output->data[4] = direction;

    output->data_len = 5;

    return true;
}

bool build_radi_info_msg(uint32_t timestamp,
                         uint8_t sensor_identifier,
                         uint16_t adc_value,
                         can_msg_t *output)
 {
    if (!output) { return false; }

    output->sid = MSG_RADI_VALUE | BOARD_UNIQUE_ID;
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = sensor_identifier;
    output->data[4] = (uint8_t) ((adc_value >> 8) & 0x0F); // Upper byte
    output->data[5] = (uint8_t) (adc_value & 0xFF); // Lower byte

    output->data_len = 6;

    return true;
 }

bool get_fill_info(const can_msg_t *msg,
                   uint8_t *lvl,
                   uint8_t *direction)
{
    if (!msg | !lvl | !direction) { return false; }

    uint16_t msg_type = get_message_type(msg);
    if (msg_type == MSG_FILL_LVL) {
        *lvl = msg->data[3];
        *direction = msg->data[4];
        return true;
    }

    return false;
}

int get_general_cmd_type(const can_msg_t *msg) {
    if (!msg) { return -1; }

    uint16_t msg_type = get_message_type(msg);
    if (msg_type == MSG_GENERAL_CMD) {
        return msg->data[3];
    } else {
        return -1;
    }
}

int get_reset_board_id(const can_msg_t *msg){
    if (!msg) { return -1; }

    uint16_t msg_type = get_message_type(msg);
    if (msg_type == MSG_RESET_CMD) {
        return msg->data[3];
    } else {
        // not a valid field for this message type
        return -1;
    }
}

int get_curr_valve_state(const can_msg_t *msg)
{
    if (!msg) { return -1; }

    uint16_t msg_type = get_message_type(msg);
    if (msg_type == MSG_VENT_VALVE_STATUS || msg_type == MSG_INJ_VALVE_STATUS) {
        return msg->data[3];
    } else {
        // not a valid field for this message type
        return -1;
    }
}

int get_req_valve_state(const can_msg_t *msg)
{
    if (!msg) { return -1; }

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

bool get_curr_arm_state(const can_msg_t *msg, uint8_t *alt_num, enum ARM_STATE *arm_state)
{
    if( !msg || !alt_num || !arm_state) { return false; }
    if(get_message_type(msg) != MSG_ALT_ARM_STATUS) { return false; }
    *alt_num = msg->data[3] & 0x0F;
    *arm_state = msg->data[3] >> 4;

    return true;
}

bool get_req_arm_state(const can_msg_t *msg, uint8_t *alt_num, enum ARM_STATE *arm_state)
{
    if( !msg || !alt_num || !arm_state) { return false; }
    if(get_message_type(msg) != MSG_ALT_ARM_CMD) { return false; }
    *alt_num = msg->data[3] & 0x0F;
    *arm_state = msg->data[3] >> 4;

    return true;
}

uint16_t get_message_type(const can_msg_t *msg)
{
    // invalid SID
    if (!msg) { return 0; }

    return (msg->sid & 0x7E0);
}

uint8_t get_board_unique_id(const can_msg_t *msg)
{
    // invalid SID
    if (!msg) { return 0; }

    return ((uint8_t) (msg->sid & 0x1F));
}

uint32_t get_timestamp(const can_msg_t *msg)
{
    // the best we can do, really
    if (!msg) { return 0; }

    uint16_t msg_type = get_message_type(msg);
    switch(msg_type) {
        // 3 byte timestamp
        case MSG_GENERAL_CMD:
        case MSG_INJ_VALVE_CMD:
        case MSG_VENT_VALVE_CMD:
        case MSG_ALT_ARM_CMD:
        case MSG_DEBUG_MSG:
        case MSG_INJ_VALVE_STATUS:
        case MSG_VENT_VALVE_STATUS:
        case MSG_ALT_ARM_STATUS:
        case MSG_GENERAL_BOARD_STATUS:
        case MSG_GPS_TIMESTAMP:
        case MSG_GPS_LATITUDE:
        case MSG_GPS_LONGITUDE:
        case MSG_GPS_ALTITUDE:
        case MSG_GPS_INFO:
        case MSG_RESET_CMD:
        case MSG_FILL_LVL:
        case MSG_SENSOR_ALTITUDE:
        case MSG_RADI_VALUE:
            return (uint32_t)msg->data[0] << 16
                   | (uint32_t)msg->data[1] << 8
                   | msg->data[2];

        // 2 byte timestamp
        case MSG_SENSOR_ACC:
        case MSG_SENSOR_GYRO:
        case MSG_SENSOR_MAG:
        case MSG_SENSOR_ANALOG:
            return (uint32_t)msg->data[0] << 8
                   | msg->data[1];

        // no timestamp
        case MSG_DEBUG_PRINTF:
        case MSG_LEDS_ON:
        case MSG_LEDS_OFF:
            return 0;

        // unknown message type
        default:
            // not much else we can do
            return 0;
    }
}

bool is_sensor_data(const can_msg_t *msg)
{
    if (!msg) { return false; }

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

bool get_imu_data(const can_msg_t *msg, uint16_t *output_x, uint16_t *output_y, uint16_t *output_z)
{
    if (!msg) { return false; }
    if (!output_x) { return false; }
    if (!output_y) { return false; }
    if (!output_z) { return false; }
    if (!is_sensor_data(msg)) { return false; }
    if (get_message_type(msg) == MSG_SENSOR_ANALOG) { return false; }

    *output_x = (uint16_t)msg->data[2] << 8 | msg->data[3];    // x
    *output_y = (uint16_t)msg->data[4] << 8 | msg->data[5];    // y
    *output_z = (uint16_t)msg->data[6] << 8 | msg->data[7];    // z

    return true;
}

bool get_analog_data(const can_msg_t *msg, enum SENSOR_ID *sensor_id, uint16_t *output_data)
{
    if (!msg) { return false; }
    if (!output_data) { return false; }
    if (get_message_type(msg) != MSG_SENSOR_ANALOG) { return false; }

    *sensor_id = msg->data[2];
    *output_data = (uint16_t)msg->data[3] << 8 | msg->data[4];

    return true;
}

bool get_altitude_data(const can_msg_t *msg, int32_t *altitude)
{
    if (!msg || !altitude) { return false; }
    if (get_message_type(msg) != MSG_SENSOR_ALTITUDE) { return false; }

    *altitude = ((uint32_t)msg->data[3] << 24);
    *altitude |= ((uint32_t)msg->data[4] << 16);
    *altitude |= ((uint32_t)msg->data[5] << 8);
    *altitude |= msg->data[6];

    return true;
}

bool get_pyro_voltage_data(const can_msg_t *msg,
                           uint16_t *v_drogue,
                           uint16_t *v_main)
{
    if (!msg || !v_drogue || !v_main) { return false; }
    if (get_message_type(msg) != MSG_ALT_ARM_STATUS) { return false; }

    *v_drogue = (msg->data[4] << 8);
    *v_drogue += msg->data[5];
    *v_main = (msg->data[6] << 8);
    *v_main += msg->data[7];

    return true;
}

bool get_gps_time(const can_msg_t *msg,
                  uint8_t *utc_hours,
                  uint8_t *utc_mins,
                  uint8_t *utc_secs,
                  uint8_t *utc_dsecs)
{
    if (!msg) { return false; }
    if (!utc_hours) { return false; }
    if (!utc_mins) { return false; }
    if (!utc_secs) { return false; }
    if (!utc_dsecs) { return false; }
    if (get_message_type(msg) != MSG_GPS_TIMESTAMP) { return false; }

    *utc_hours = msg->data[3];
    *utc_mins  = msg->data[4];
    *utc_secs  = msg->data[5];
    *utc_dsecs = msg->data[6];

    return true;
}

bool get_gps_lat(const can_msg_t *msg,
                 uint8_t *degrees,
                 uint8_t *minutes,
                 uint16_t *dminutes,
                 uint8_t *direction)
{
    if (!msg) { return false; }
    if (!degrees) { return false; }
    if (!minutes) { return false; }
    if (!dminutes) { return false; }
    if (!direction) { return false; }
    if (get_message_type(msg) != MSG_GPS_LATITUDE) { return false; }

    *degrees = msg->data[3];
    *minutes = msg->data[4];
    *dminutes = msg->data[5] << 8 | msg->data[6];
    *direction = msg->data[7];

    return true;
}

bool get_gps_lon(const can_msg_t *msg,
                 uint8_t *degrees,
                 uint8_t *minutes,
                 uint16_t *dminutes,
                 uint8_t *direction)
{
    if (!msg) { return false; }
    if (!degrees) { return false; }
    if (!minutes) { return false; }
    if (!dminutes) { return false; }
    if (!direction) { return false; }
    if (get_message_type(msg) != MSG_GPS_LONGITUDE) { return false; }

    *degrees = msg->data[3];
    *minutes = msg->data[4];
    *dminutes = msg->data[5] << 8 | msg->data[6];
    *direction = msg->data[7];

    return true;
}

bool get_gps_alt(const can_msg_t *msg,
                 uint16_t *altitude,
                 uint8_t *daltitude,
                 uint8_t *units)
{
    if (!msg) { return false; }
    if (!altitude) { return false; }
    if (!daltitude) { return false; }
    if (!units) { return false; }
    if (get_message_type(msg) != MSG_GPS_ALTITUDE) { return false; }

    *altitude = (uint16_t)msg->data[3] << 8 | msg->data[4];
    *daltitude = msg->data[5];
    *units = msg->data[6];

    return true;
}

bool get_gps_info(const can_msg_t *msg,
                 uint8_t *num_sat,
                 uint8_t *quality)
{
    if (!msg) { return false; }
    if (!num_sat) { return false; }
    if (!quality) { return false; }
    if (get_message_type(msg) != MSG_GPS_INFO) { return false; }

    *num_sat = msg->data[3];
    *quality = msg->data[4];

    return true;
}

bool get_radi_info(const can_msg_t* msg,
                   uint8_t *sensor_identifier,
                   uint16_t *adc_value)
{
    if (!msg) { return false; }
    if (!sensor_identifier) { return false; }
    if (!adc_value) { return false; }
    if (get_message_type(msg) != MSG_RADI_VALUE) {return false;}

    *sensor_identifier = msg -> data[3];
    *adc_value = msg -> data[4] << 8 | msg -> data[5];
    return true;
}

can_debug_level_t message_debug_level(const can_msg_t *msg)
{
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

const char *build_radio_cmd_can_message(const char *string, can_msg_t *output)
{
    // let build_printf_can_message do all the heavy lifting
    string = build_printf_can_message(string, output);
    // then just set SID, since that's the only difference in message type
    output->sid = (MSG_DEBUG_RADIO_CMD | BOARD_UNIQUE_ID);
    return string;
}
