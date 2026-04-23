#ifndef CANLIB_ACTUATOR_H
#define CANLIB_ACTUATOR_H

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

#include "can.h"
#include "message_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void build_actuator_cmd_msg(can_msg_prio_t prio, uint16_t timestamp, can_actuator_id_t actuator_id,
							can_actuator_state_t actuator_cmd, can_msg_t *output);

void build_actuator_status_msg(can_msg_prio_t prio, uint16_t timestamp,
							   can_actuator_id_t actuator_id,
							   can_actuator_state_t actuator_cmd_state,
							   can_actuator_state_t actuator_curr_state, can_msg_t *output);

/*
 * Returns the actuator id from an actuator command or status message.
 */
w_status_t get_actuator_id(const can_msg_t *msg, can_actuator_id_t *actuator_id);

/*
 * Returns the current actuator state based on limit switch readings.
 */
w_status_t get_curr_actuator_state(const can_msg_t *msg, can_actuator_state_t *curr_actuator_state);

/*
 * Returns the commanded actuator state from an actuator command or status message.
 */
w_status_t get_cmd_actuator_state(const can_msg_t *msg, can_actuator_state_t *cmd_actuator_state);

#ifdef __cplusplus
}
#endif

#endif
