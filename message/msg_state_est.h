#ifndef CANLIB_STATE_EST_H
#define CANLIB_STATE_EST_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

/*
 * Used to build state estimation data
 */
bool build_state_est_data_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, const float *data, enum STATE_ID data_id,
    can_msg_t *output
);

/*
 * Used to build state estimation calibration message
 */
bool build_state_est_calibration_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, uint8_t ack_flag, uint16_t apogee, can_msg_t *output
);

/*
 * Gets state estimation data. Returns true if
 * successful, false if the input is invalid.
 */
bool get_state_est_data(const can_msg_t *msg, float *data, enum STATE_ID *data_id);
/*
 * Get state estimation calibration message
 */
bool get_state_est_calibration_msg(const can_msg_t *msg, uint8_t *ack_flag, uint16_t *apogee);

#endif
