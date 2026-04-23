#include <stdbool.h>
#include <stdint.h>

#include "common.h"

#include "can.h"
#include "message_types.h"
#include "msg_actuator.h"
#include "msg_common.h"

void build_actuator_cmd_msg(can_msg_prio_t prio, uint16_t timestamp, can_actuator_id_t actuator_id,
							can_actuator_state_t actuator_cmd, can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_ACTUATOR_CMD, actuator_id);
	write_timestamp(timestamp, output);

	output->data[2] = (uint8_t)actuator_cmd;
	output->data_len = 3;
}

void build_actuator_status_msg(can_msg_prio_t prio, uint16_t timestamp,
							   can_actuator_id_t actuator_id,
							   can_actuator_state_t actuator_cmd_state,
							   can_actuator_state_t actuator_curr_state, can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_ACTUATOR_STATUS, actuator_id);
	write_timestamp(timestamp, output);

	output->data[2] = (uint8_t)actuator_cmd_state;
	output->data[3] = (uint8_t)actuator_curr_state;
	output->data_len = 4;
}

w_status_t get_actuator_id(const can_msg_t *msg, can_actuator_id_t *actuator_id) {
	w_assert(msg);
	w_assert(actuator_id);

	*actuator_id = (can_actuator_id_t)get_message_metadata(msg);

	can_msg_type_t msg_type = get_message_type(msg);

	if (msg_type == MSG_ACTUATOR_CMD) {
		if (msg->data_len == 3) {
			return W_SUCCESS;
		} else {
			return W_DATA_FORMAT_ERROR;
		}
	} else if (msg_type == MSG_ACTUATOR_STATUS) {
		if (msg->data_len == 4) {
			return W_SUCCESS;
		} else {
			return W_DATA_FORMAT_ERROR;
		}
	}

	return W_INVALID_PARAM;
}

w_status_t get_curr_actuator_state(const can_msg_t *msg,
								   can_actuator_state_t *curr_actuator_state) {
	w_assert(msg);
	w_assert(curr_actuator_state);

	*curr_actuator_state = (can_actuator_state_t)msg->data[3];

	can_msg_type_t msg_type = get_message_type(msg);

	if (msg_type != MSG_ACTUATOR_STATUS) {
		return W_INVALID_PARAM;
	}

	if (msg->data_len == 4) {
		return W_SUCCESS;
	} else {
		return W_DATA_FORMAT_ERROR;
	}

	return W_SUCCESS;
}

w_status_t get_cmd_actuator_state(const can_msg_t *msg, can_actuator_state_t *cmd_actuator_state) {
	w_assert(msg);
	w_assert(cmd_actuator_state);

	*cmd_actuator_state = (can_actuator_state_t)msg->data[2];

	can_msg_type_t msg_type = get_message_type(msg);

	if (msg_type == MSG_ACTUATOR_CMD) {
		if (msg->data_len == 3) {
			return W_SUCCESS;
		} else {
			return W_DATA_FORMAT_ERROR;
		}
	} else if (msg_type == MSG_ACTUATOR_STATUS) {
		if (msg->data_len == 4) {
			return W_SUCCESS;
		} else {
			return W_DATA_FORMAT_ERROR;
		}
	}

	return W_INVALID_PARAM;
}
