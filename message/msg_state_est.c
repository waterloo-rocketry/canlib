#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "can.h"
#include "common.h"
#include "message_types.h"
#include "msg_state_est.h"

bool build_state_est_data_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, const float *data, enum STATE_ID data_id,
    can_msg_t *output
) {
    output->sid = SID(prio, MSG_STATE_EST_DATA);
    write_timestamp_3bytes(timestamp, output);
    memcpy(output->data + 3, data, sizeof(*data));
    output->data[7] = data_id;
    output->data_len = 8;

    return true;
}

bool build_state_est_calibration_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, uint8_t ack_flag, uint16_t apogee, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_STATE_EST_CALIB);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = ack_flag;
    output->data[4] = (apogee >> 8) & 0xff;
    output->data[5] = apogee & 0xff;

    output->data_len = 6;

    return true;
}

bool get_state_est_data(const can_msg_t *msg, float *data, enum STATE_ID *data_id) {
    if (!msg) {
        return false;
    }
    if (!data) {
        return false;
    }
    if (!data_id) {
        return false;
    }
    if (get_message_type(msg) != MSG_STATE_EST_DATA) {
        return false;
    }

    memcpy(data, msg->data + 3, sizeof(*data));
    *data_id = msg->data[7];

    return true;
}

bool get_state_est_calibration_msg(const can_msg_t *msg, uint8_t *ack_flag, uint16_t *apogee) {
    if (!msg) {
        return false;
    }
    if (!ack_flag) {
        return false;
    }
    if (!apogee) {
        return false;
    }
    if (get_message_type(msg) != MSG_STATE_EST_CALIB) {
        return false;
    }

    *ack_flag = msg->data[3];
    *apogee = msg->data[4] << 8 | msg->data[5];

    return true;
}
