#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_state_est.h"

bool build_state_est_data_msg(can_msg_prio_t prio, uint16_t timestamp,
                              can_state_est_id_t state_id,
                              const float *state_data, can_msg_t *output) {
  output->sid = SID(prio, MSG_STATE_EST_DATA);
  write_timestamp_2bytes(timestamp, output);
  const uint8_t *data = (const uint8_t *)state_data;

  output->data[2] = state_id;
  output->data[3] = data[3];
  output->data[4] = data[2];
  output->data[5] = data[1];
  output->data[6] = data[0];

  output->data_len = 7;

  return true;
}

bool get_state_est_data(const can_msg_t *msg, can_state_est_id_t *state_id,
                        float *state_data) {
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
  uint8_t *data = (uint8_t *)state_data;
  data[0] = msg->data[6];
  data[1] = msg->data[5];
  data[2] = msg->data[4];
  data[3] = msg->data[3];

  return true;
}
