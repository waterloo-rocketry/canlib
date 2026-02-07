#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_recovery.h"

bool build_alt_arm_cmd_msg(can_msg_prio_t prio, uint16_t timestamp, can_altimeter_id_t alt_id,
						   can_alt_arm_state_t arm_cmd, can_msg_t *output) {
	if (!output) {
		return false;
	}

	output->sid = build_sid(prio, MSG_ALT_ARM_CMD, alt_id);
	write_timestamp(timestamp, output);

	output->data[2] = arm_cmd;
	output->data_len = 3;

	return true;
}

bool build_alt_arm_status_msg(can_msg_prio_t prio, uint16_t timestamp, can_altimeter_id_t alt_id,
							  can_alt_arm_state_t arm_state, uint16_t v_drogue, uint16_t v_main,
							  can_msg_t *output) {
	if (!output) {
		return false;
	}

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

	return true;
}

bool get_alt_arm_state(const can_msg_t *msg, can_altimeter_id_t *alt_id,
					   can_alt_arm_state_t *arm_state) {
	if (!msg || !alt_id || !arm_state) {
		return false;
	}
	if ((get_message_type(msg) != MSG_ALT_ARM_CMD) &&
		(get_message_type(msg) != MSG_ALT_ARM_STATUS)) {
		return false;
	}
	*alt_id = get_message_metadata(msg);
	*arm_state = msg->data[2];

	return true;
}

bool get_pyro_voltage_data(const can_msg_t *msg, uint16_t *v_drogue, uint16_t *v_main) {
	if (!msg || !v_drogue || !v_main) {
		return false;
	}
	if (get_message_type(msg) != MSG_ALT_ARM_STATUS) {
		return false;
	}

	*v_drogue = (msg->data[4] << 8);
	*v_drogue += msg->data[5];
	*v_main = (msg->data[6] << 8);
	*v_main += msg->data[7];

	return true;
}

