#ifndef CANLIB_STATE_EST_H
#define CANLIB_STATE_EST_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Used to build state estimation data
 */
bool build_state_est_data_msg(can_msg_prio_t prio, uint16_t timestamp,
                              can_state_est_id_t state_id,
                              const float *state_data, can_msg_t *output);

/*
 * Gets state estimation data. Returns true if
 * successful, false if the input is invalid.
 */
bool get_state_est_data(const can_msg_t *msg, can_state_est_id_t *state_id,
                        float *state_data);

#ifdef __cplusplus
}
#endif

#endif
