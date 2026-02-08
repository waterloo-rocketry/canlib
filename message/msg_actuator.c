#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_actuator.h"
#include "msg_common.h"

bool build_actuator_cmd_msg(can_msg_prio_t prio, uint16_t timestamp, can_actuator_id_t actuator_id,
							can_actuator_state_t actuator_cmd, can_msg_t *output) {
	if (!output) {
		return false;
	}

	output->sid = build_sid(prio, MSG_ACTUATOR_CMD, actuator_cmd);
	write_timestamp(timestamp, output);

	output->data[2] = (uint8_t)actuator_cmd;
	output->data_len = 3;

	return true;
}

bool build_actuator_status_msg(can_msg_prio_t prio, uint16_t timestamp,
							   can_actuator_id_t actuator_id,
							   can_actuator_state_t actuator_curr_state,
							   can_actuator_state_t actuator_cmd_state, can_msg_t *output) {
	if (!output) {
		return false;
	}

	output->sid = build_sid(prio, MSG_ACTUATOR_STATUS, actuator_id);
	write_timestamp(timestamp, output);

	output->data[2] = (uint8_t)actuator_curr_state;
	output->data[3] = (uint8_t)actuator_cmd_state;
	output->data_len = 4;

	return true;
}

int get_actuator_id(const can_msg_t *msg) {
	if (!msg) {
		return -1;
	}

	uint16_t msg_type = get_message_type(msg);
	switch (msg_type) {
		case MSG_ACTUATOR_CMD:
		case MSG_ACTUATOR_STATUS:

			return get_message_metadata(msg);

		default:
			// not a valid field for this message type
			return -1;
	}
}

int get_curr_actuator_state(const can_msg_t *msg) {
	if (!msg) {
		return -1;
	}

	uint16_t msg_type = get_message_type(msg);
	if (msg_type == MSG_ACTUATOR_STATUS) {
		return msg->data[2];
	} else {
		// not a valid field for this message type
		return -1;
	}
}

int get_cmd_actuator_state(const can_msg_t *msg) {
	if (!msg) {
		return -1;
	}

	uint16_t msg_type = get_message_type(msg);
	switch (msg_type) {
		case MSG_ACTUATOR_STATUS:
			return msg->data[3];

		case MSG_ACTUATOR_CMD:
			return msg->data[2];

		default:
			// not a valid field for this message type
			return -1;
	}
}
