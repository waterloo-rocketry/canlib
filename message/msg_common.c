#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"

// Helper function for populating CAN messages
void write_timestamp_2bytes(uint16_t timestamp, can_msg_t *output) {
    output->data[0] = (timestamp >> 8) & 0xff;
    output->data[1] = (timestamp >> 0) & 0xff;
}

void write_timestamp_3bytes(uint32_t timestamp, can_msg_t *output) {
    output->data[0] = (timestamp >> 16) & 0xff;
    output->data[1] = (timestamp >> 8) & 0xff;
    output->data[2] = (timestamp >> 0) & 0xff;
}

uint16_t get_message_type(const can_msg_t *msg) {
    // invalid SID
    if (!msg) {
        return 0;
    }

    return (uint16_t)((msg->sid >> 18) & 0x1FF);
}

uint8_t get_board_type_unique_id(const can_msg_t *msg) {
    // invalid SID
    if (!msg) {
        return 0;
    }

    return (uint8_t)((msg->sid >> 8) & 0xFF);
}

uint8_t get_board_inst_unique_id(const can_msg_t *msg) {
    // invalid SID
    if (!msg) {
        return 0;
    }

    return (uint8_t)(msg->sid & 0xFF);
}

uint32_t get_timestamp(const can_msg_t *msg) {
    // the best we can do, really
    if (!msg) {
        return 0;
    }

    uint16_t msg_type = get_message_type(msg);
    switch (msg_type) {
        // 3 byte timestamp
        case MSG_GENERAL_CMD:
        case MSG_ACTUATOR_CMD:
        case MSG_ALT_ARM_CMD:
        case MSG_DEBUG_MSG:
        case MSG_ACTUATOR_STATUS:
        case MSG_ALT_ARM_STATUS:
        case MSG_GENERAL_BOARD_STATUS:
        case MSG_GPS_TIMESTAMP:
        case MSG_GPS_LATITUDE:
        case MSG_GPS_LONGITUDE:
        case MSG_SENSOR_TEMP:
        case MSG_GPS_ALTITUDE:
        case MSG_GPS_INFO:
        case MSG_RESET_CMD:
        case MSG_FILL_LVL:
        case MSG_SENSOR_ALTITUDE:
        case MSG_STATE_EST_DATA:
        case MSG_ACT_ANALOG_CMD:
        case MSG_STATE_EST_CALIB:
            return (uint32_t)msg->data[0] << 16 | msg->data[1] << 8 | msg->data[2];

        // 2 byte timestamp
        case MSG_SENSOR_ACC:
        case MSG_SENSOR_ACC2:
        case MSG_SENSOR_GYRO:
        case MSG_SENSOR_MAG:
        case MSG_SENSOR_ANALOG:
            return (uint32_t)msg->data[0] << 8 | msg->data[1];

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
