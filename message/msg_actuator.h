#ifndef CANLIB_ACTUATOR_H
#define CANLIB_ACTUATOR_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void build_actuator_cmd_msg(can_msg_prio_t prio, uint16_t timestamp, can_actuator_id_t actuator_id,
							can_actuator_state_t actuator_cmd, can_msg_t *output);

void build_actuator_status_msg(can_msg_prio_t prio, uint16_t timestamp,
							   can_actuator_id_t actuator_id,
							   can_actuator_state_t actuator_curr_state,
							   can_actuator_state_t actuator_cmd_state, can_msg_t *output);

/*
 * Returns the actuator id from an actuator command or status message.
 * Returns -1 if the provided message is not an actuator cmd/status.
 */
int get_actuator_id(const can_msg_t *msg);

/*
 * Returns the current actuator state based on limit switch readings.
 * Returns -1 if the provided message is not a actuator status message.
 */
int get_curr_actuator_state(const can_msg_t *msg);

/*
 * Returns the commanded actuator state from an actuator command or
 * status message. Returns -1 if the provided message is not
 * an actuator cmd/status.
 */
int get_cmd_actuator_state(const can_msg_t *msg);

#ifdef __cplusplus
}
#endif

#endif
