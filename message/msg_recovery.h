#ifndef CANLIB_RECOVERY_H
#define CANLIB_RECOVERY_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

/*
 * Used to send altimeter arm commands
 */
bool build_arm_cmd_msg(
    can_msg_prio_t prio, uint32_t timestamp, uint8_t alt_num, enum ARM_STATE arm_cmd,
    can_msg_t *output
);

/*
 * Used to send the current altimeter arming status
 */
bool build_arm_stat_msg(
    can_msg_prio_t prio, uint32_t timestamp, uint8_t alt_num, enum ARM_STATE arm_state,
    uint16_t v_drogue, uint16_t v_main, can_msg_t *output
);

/*
 * Gets the current arm state and which altimeter it is for.
 * Returns false if the provided message is not an arm status.
 */
bool get_curr_arm_state(const can_msg_t *msg, uint8_t *alt_num, enum ARM_STATE *arm_state);

/*
 * Gets the requested arm state and altimeter number.
 * Returns false if the provided message is not an arm state request."
 */
bool get_req_arm_state(const can_msg_t *msg, uint8_t *alt_num, enum ARM_STATE *arm_state);

/*
 * Gets the voltage of the drogue and main pyro lines, returns false
 * if the message is not an ALT_ARM_STATUS message
 */
bool get_pyro_voltage_data(const can_msg_t *msg, uint16_t *v_drogue, uint16_t *v_main);

#endif
