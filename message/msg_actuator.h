#ifndef CANLIB_ACTUATOR_H
#define CANLIB_ACTUATOR_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

/*
 * Used to send injector and vent commands
 */
bool build_actuator_cmd_msg(
    can_msg_prio_t prio, uint32_t timestamp, enum ACTUATOR_ID actuator_id,
    enum ACTUATOR_STATE actuator_cmd, can_msg_t *output
);

/*
 * Used to send injector/vent status: current and desired
 */
bool build_actuator_stat_msg(
    can_msg_prio_t prio, uint32_t timestamp, enum ACTUATOR_ID actuator_id,
    enum ACTUATOR_STATE actuator_state, enum ACTUATOR_STATE req_actuator_state, can_msg_t *output
);

/*
 * Used to send motor control commands
 */
bool build_actuator_cmd_analog(
    can_msg_prio_t prio, uint32_t timestamp, enum ACTUATOR_ID actuator_id, uint8_t actuator_cmd,
    can_msg_t *output
);

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
 * Returns the requested actuator state from an actuator command or
 * status message. Returns -1 if the provided message is not
 * an actuator cmd/status.
 */
int get_req_actuator_state(const can_msg_t *msg);

/*
 * Returns the requested actuator state from an analog actuator
 * command or status message. Returns -1.0 if the provided message is not
 * an actuator cmd/status.
 */
uint8_t get_req_actuator_state_analog(const can_msg_t *msg);

#endif
