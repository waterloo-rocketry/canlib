#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_state_est.h"

bool build_state_est_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, can_state_est_id_t state_id, const float *state_data,
    can_msg_t *output
) {
    output->sid = SID(prio, MSG_STATE_EST_DATA);
    write_timestamp_2bytes(timestamp, output);
    output->data[2] = state_id;
    memcpy(output->data + 3, state_data, sizeof(*state_data));

    output->data_len = 8;

    return true;
}

bool get_state_est_data(const can_msg_t *msg, can_state_est_id_t *state_id, float *state_data) {
    if (!msg) {
        return false;
    }
    if (!state_id) {
        return false;
    }
    if (!state_data) {
        return false;
    }
    if (get_message_type(msg) != MSG_STATE_EST_DATA) {
        return false;
    }

    *state_id = msg->data[2];
    memcpy(state_data, msg->data + 3, sizeof(*state_data));

    return true;
}
