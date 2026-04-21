#include <stdint.h>

#include "common.h"

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_recovery.h"

void build_alt_arm_cmd_msg(can_msg_prio_t prio, uint16_t timestamp, can_altimeter_id_t alt_id,
						   can_alt_arm_state_t arm_cmd, can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_ALT_ARM_CMD, alt_id);
	write_timestamp(timestamp, output);

	output->data[2] = arm_cmd;
	output->data_len = 3;
}

void build_alt_arm_status_msg(can_msg_prio_t prio, uint16_t timestamp, can_altimeter_id_t alt_id,
							  can_alt_arm_state_t arm_state, uint16_t v_drogue, uint16_t v_main,
							  can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_ALT_ARM_STATUS, alt_id);
	write_timestamp(timestamp, output);

	output->data[2] = arm_state;
	// drogue voltage
	output->data[3] = v_drogue >> 8; // 8 msb
	output->data[4] = v_drogue & 0x00FF; // 8 lsb
	// main voltage
	output->data[5] = v_main >> 8; // 8 msb
	output->data[6] = v_main & 0x00FF; // 8 lsb

	output->data_len = 7;
}

w_status_t get_alt_arm_state(const can_msg_t *msg, can_altimeter_id_t *alt_id,
							 can_alt_arm_state_t *arm_state) {
	w_assert(msg);
	w_assert(alt_id);
	w_assert(arm_state);

	*alt_id = (can_altimeter_id_t)get_message_metadata(msg);
	*arm_state = (can_alt_arm_state_t)msg->data[2];

	uint16_t msg_type = get_message_type(msg);
	if (msg_type == MSG_ALT_ARM_CMD) {
		if (msg->data_len == 3) {
			return W_SUCCESS;
		} else {
			return W_DATA_FORMAT_ERROR;
		}
	} else if (msg_type == MSG_ALT_ARM_STATUS) {
		if (msg->data_len == 7) {
			return W_SUCCESS;
		} else {
			return W_DATA_FORMAT_ERROR;
		}
	}

	return W_INVALID_PARAM;
}

w_status_t get_pyro_voltage_data(const can_msg_t *msg, uint16_t *v_drogue, uint16_t *v_main) {
	w_assert(msg);
	w_assert(v_drogue);
	w_assert(v_main);

	*v_drogue = (msg->data[3] << 8);
	*v_drogue += msg->data[4];
	*v_main = (msg->data[5] << 8);
	*v_main += msg->data[6];

	if (get_message_type(msg) != MSG_ALT_ARM_STATUS) {
		return W_INVALID_PARAM;
	}

	if (msg->data_len != 7) {
		return W_DATA_FORMAT_ERROR;
	}

	return W_SUCCESS;
}

