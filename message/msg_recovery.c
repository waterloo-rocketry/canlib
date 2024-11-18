#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_recovery.h"

bool build_arm_cmd_msg(
    can_msg_prio_t prio, uint32_t timestamp, uint8_t alt_num, enum ARM_STATE arm_cmd,
    can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_ALT_ARM_CMD);
    write_timestamp_3bytes(timestamp, output);

    // 4 msb are used for arm state, 4 lsb used for altimeter number
    output->data[3] = (arm_cmd << 4) | (alt_num & 0x0F);
    output->data_len = 4; // 3 bytes timestamp, 1 byte arm state + alt num

    return true;
}

bool build_arm_stat_msg(
    can_msg_prio_t prio, uint32_t timestamp, uint8_t alt_num, enum ARM_STATE arm_state,
    uint16_t v_drogue, uint16_t v_main, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_ALT_ARM_STATUS);
    write_timestamp_3bytes(timestamp, output);

    // 4 msb are used for arm state, 4 lsb used for altimeter number
    output->data[3] = (arm_state << 4) | (alt_num & 0x0F);
    // drogue voltage
    output->data[4] = v_drogue >> 8; // 8 msb
    output->data[5] = v_drogue & 0x00FF; // 8 lsb
    // main voltage
    output->data[6] = v_main >> 8; // 8 msb
    output->data[7] = v_main & 0x00FF; // 8 lsb
    // 3 bytes timestamp, 1 byte arm state + alt num, 4 bytes voltages
    output->data_len = 8;

    return true;
}

bool get_curr_arm_state(const can_msg_t *msg, uint8_t *alt_num, enum ARM_STATE *arm_state) {
    if (!msg || !alt_num || !arm_state) {
        return false;
    }
    if (get_message_type(msg) != MSG_ALT_ARM_STATUS) {
        return false;
    }
    *alt_num = msg->data[3] & 0x0F;
    *arm_state = msg->data[3] >> 4;

    return true;
}

bool get_req_arm_state(const can_msg_t *msg, uint8_t *alt_num, enum ARM_STATE *arm_state) {
    if (!msg || !alt_num || !arm_state) {
        return false;
    }
    if (get_message_type(msg) != MSG_ALT_ARM_CMD) {
        return false;
    }
    *alt_num = msg->data[3] & 0x0F;
    *arm_state = msg->data[3] >> 4;

    return true;
}

bool get_pyro_voltage_data(const can_msg_t *msg, uint16_t *v_drogue, uint16_t *v_main) {
    if (!msg || !v_drogue || !v_main) {
        return false;
    }
    if (get_message_type(msg) != MSG_ALT_ARM_STATUS) {
        return false;
    }

    *v_drogue = (msg->data[4] << 8);
    *v_drogue += msg->data[5];
    *v_main = (msg->data[6] << 8);
    *v_main += msg->data[7];

    return true;
}

