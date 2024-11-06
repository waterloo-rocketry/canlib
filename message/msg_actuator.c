#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_actuator.h"
#include "msg_common.h"

bool build_actuator_cmd_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, enum ACTUATOR_ID actuator_id,
    enum ACTUATOR_STATE actuator_cmd, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_ACTUATOR_CMD);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = (uint8_t)actuator_id;
    output->data[4] = (uint8_t)actuator_cmd;
    output->data_len = 5; // 3 bytes timestamp, 2 byte data

    return true;
}

bool build_actuator_stat_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, enum ACTUATOR_ID actuator_id,
    enum ACTUATOR_STATE actuator_state, enum ACTUATOR_STATE req_actuator_state, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_ACTUATOR_STATUS);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = (uint8_t)actuator_id;
    output->data[4] = (uint8_t)actuator_state;
    output->data[5] = (uint8_t)req_actuator_state;
    output->data_len = 6; // 3 bytes timestamp, 3 bytes data

    return true;
}

bool build_actuator_cmd_analog(
    enum MESSAGE_PRIO prio, uint32_t timestamp, enum ACTUATOR_ID actuator_id, uint8_t actuator_cmd,
    can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_ACT_ANALOG_CMD);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = actuator_id;
    output->data[4] = actuator_cmd;

    output->data_len = 5;

    return true;
}

int get_actuator_id(const can_msg_t *msg) {
    if (!msg) {
        return -1;
    }

    uint16_t msg_type = get_message_type(msg);
    switch (msg_type) {
        case MSG_ACTUATOR_CMD:
        case MSG_ACTUATOR_STATUS:
        case MSG_ACT_ANALOG_CMD:
            return msg->data[3];

        default:
            // not a valid field for this message type
            return -1;
    }
}

int get_curr_actuator_state(const can_msg_t *msg) {
    if (!msg) {
        return -1;
    }

    uint16_t msg_type = get_message_type(msg);
    if (msg_type == MSG_ACTUATOR_STATUS) {
        return msg->data[4];
    } else {
        // not a valid field for this message type
        return -1;
    }
}

int get_req_actuator_state(const can_msg_t *msg) {
    if (!msg) {
        return -1;
    }

    uint16_t msg_type = get_message_type(msg);
    switch (msg_type) {
        case MSG_ACTUATOR_STATUS:
            return msg->data[5];

        case MSG_ACTUATOR_CMD:
            return msg->data[4];

        default:
            // not a valid field for this message type
            return -1;
    }
}

uint8_t get_req_actuator_state_analog(const can_msg_t *msg) {
    if (!msg) {
        return 0;
    }

    uint16_t msg_type = get_message_type(msg);
    switch (msg_type) {
        case MSG_ACT_ANALOG_CMD:
            return msg->data[4];

        default:
            // not a valid field for this message type
            return 0;
    }
}
