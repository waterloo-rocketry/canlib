#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_actuator.h"
#include "msg_common.h"

bool build_actuator_cmd_msg(
    can_msg_prio_t prio, uint16_t timestamp, can_actuator_id_t actuator_id,
    can_actuator_state_t actuator_cmd, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_ACTUATOR_CMD);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = (uint8_t)actuator_id;
    output->data[3] = (uint8_t)actuator_cmd;
    output->data_len = 4;

    return true;
}

bool build_actuator_analog_cmd_msg(
    can_msg_prio_t prio, uint32_t timestamp, can_actuator_state_t actuator_id,
    uint16_t actuator_cmd, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_ACTUATOR_ANALOG_CMD);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = actuator_id;
    output->data[4] = (actuator_cmd >> 8) & 0xff;
    output->data[4] = actuator_cmd & 0xff;

    output->data_len = 5;

    return true;
}

bool build_actuator_status_msg(
    can_msg_prio_t prio, uint16_t timestamp, can_actuator_id_t actuator_id,
    can_actuator_state_t actuator_curr_state, can_actuator_id_t actuator_req_state,
    can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_ACTUATOR_STATUS);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = (uint8_t)actuator_id;
    output->data[3] = (uint8_t)actuator_curr_state;
    output->data[4] = (uint8_t)actuator_req_state;
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
        case MSG_ACTUATOR_ANALOG_CMD:
        case MSG_ACTUATOR_STATUS:

            return msg->data[2];

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
        return msg->data[3];
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
            return msg->data[4];

        case MSG_ACTUATOR_CMD:
            return msg->data[3];

        default:
            // not a valid field for this message type
            return -1;
    }
}

uint16_t get_req_actuator_state_analog(const can_msg_t *msg) {
    if (!msg) {
        return 0;
    }

    uint16_t msg_type = get_message_type(msg);
    switch (msg_type) {
        case MSG_ACTUATOR_ANALOG_CMD:
            return (msg->data[3] << 8) | msg->data[4];

        default:
            // not a valid field for this message type
            return 0;
    }
}
