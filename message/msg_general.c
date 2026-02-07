#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_general.h"

void build_general_board_status_msg(can_msg_prio_t prio, uint16_t timestamp,
									uint32_t general_error_bitfield,
									uint16_t board_specific_error_bitfield, can_msg_t *output) {
	assert(output);

	output->sid = build_sid(prio, MSG_GENERAL_BOARD_STATUS, 0);
	write_timestamp(timestamp, output);
	output->data[2] = (general_error_bitfield >> 24) & 0xff;
	output->data[3] = (general_error_bitfield >> 16) & 0xff;
	output->data[4] = (general_error_bitfield >> 8) & 0xff;
	output->data[5] = general_error_bitfield & 0xff;
	output->data[6] = (board_specific_error_bitfield >> 8) & 0xff;
	output->data[7] = board_specific_error_bitfield & 0xff;
	output->data_len = 8;
}

void build_reset_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t board_type_id,
					 uint8_t board_inst_id, can_msg_t *output) {
	assert(output);

	output->sid = build_sid(prio, MSG_RESET_CMD, 0);
	write_timestamp(timestamp, output);
	output->data[2] = board_type_id;
	output->data[3] = board_inst_id;
	output->data_len = 4;
}

void build_debug_raw_msg(can_msg_prio_t prio, uint16_t timestamp, const uint8_t *data,
						 can_msg_t *output) {
	assert(output);

	output->sid = build_sid(prio, MSG_DEBUG_RAW, 0);
	write_timestamp(timestamp, output);

	memcpy(output->data + 2, data, 6);
	output->data_len = 8;
}

void build_config_set_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t board_type_id,
						  uint8_t board_inst_id, uint16_t config_id, uint16_t config_value,
						  can_msg_t *output) {
	assert(output);

	output->sid = build_sid(prio, MSG_CONFIG_SET, 0);
	write_timestamp(timestamp, output);

	output->data[2] = board_type_id;
	output->data[3] = board_inst_id;
	output->data[4] = (config_id >> 8) & 0xff;
	output->data[5] = config_id & 0xff;
	output->data[6] = (config_value >> 8) & 0xff;
	output->data[7] = config_value & 0xff;
	output->data_len = 8;
}

void build_config_status_msg(can_msg_prio_t prio, uint16_t timestamp, uint16_t config_id,
							 uint16_t config_value, can_msg_t *output) {
	assert(output);

	output->sid = build_sid(prio, MSG_CONFIG_STATUS, 0);
	write_timestamp(timestamp, output);

	output->data[2] = (config_id >> 8) & 0xff;
	output->data[3] = config_id & 0xff;
	output->data[4] = (config_value >> 8) & 0xff;
	output->data[5] = config_value & 0xff;
	output->data_len = 6;
}

bool get_general_board_status(const can_msg_t *msg, uint32_t *general_error_bitfield,
							  uint16_t *board_specific_error_bitfield) {
	assert(msg);

	uint16_t msg_type = get_message_type(msg);
	if (msg_type == MSG_GENERAL_BOARD_STATUS) {
		*general_error_bitfield =
			(msg->data[2] << 24) | (msg->data[3] << 16) | (msg->data[4] << 8) | (msg->data[5]);
		*board_specific_error_bitfield = (msg->data[6] << 8) | (msg->data[7]);
		return true;
	} else {
		return false;
	}
}

bool get_reset_board_id(const can_msg_t *msg, uint8_t *board_type_id, uint8_t *board_inst_id) {
	if (!msg) {
		return false;
	}

	uint16_t msg_type = get_message_type(msg);
	if (msg_type == MSG_RESET_CMD) {
		*board_type_id = msg->data[2];
		*board_inst_id = msg->data[3];
		return true;
	} else {
		// not a valid field for this message type
		return false;
	}
}

bool check_board_need_reset(const can_msg_t *msg) {
	uint8_t board_type_id, board_inst_id;

	if (!get_reset_board_id(msg, &board_type_id, &board_inst_id)) {
		return false;
	}

#ifndef CANLIB_DYNAMIC_BOARD_ID
	const uint8_t board_type_unique_id = BOARD_TYPE_UNIQUE_ID;
	const uint8_t board_inst_unique_id = BOARD_INST_UNIQUE_ID;
#endif

	if (board_type_id == BOARD_TYPE_ID_ANY) {
		return true;
	} else if (board_type_id == board_type_unique_id) {
		if (board_inst_id == BOARD_INST_ID_ANY) {
			return true;
		} else if (board_inst_id == board_inst_unique_id) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool get_debug_raw_data(const can_msg_t *msg, uint8_t *data) {
	if (!msg) {
		return false;
	}

	uint16_t msg_type = get_message_type(msg);
	if (msg_type == MSG_DEBUG_RAW) {
		memcpy(data, msg->data + 2, 6);
		return true;
	} else {
		return false;
	}
}

bool get_config_set_target_board(const can_msg_t *msg, uint8_t *board_type_id,
								 uint8_t *board_inst_id) {
	if (!msg) {
		return false;
	}

	uint16_t msg_type = get_message_type(msg);
	if (msg_type == MSG_CONFIG_SET) {
		*board_type_id = msg->data[2];
		*board_inst_id = msg->data[3];
		return true;
	} else {
		return false;
	}
}

bool get_config_id_value(const can_msg_t *msg, uint16_t *config_id, uint16_t *config_value) {
	if (!msg) {
		return false;
	}

	uint16_t msg_type = get_message_type(msg);
	if (msg_type == MSG_CONFIG_SET) {
		*config_id = (msg->data[4] << 8) | (msg->data[5]);
		*config_value = (msg->data[6] << 8) | (msg->data[7]);
		return true;
	} else if (msg_type == MSG_CONFIG_STATUS) {
		*config_id = (msg->data[2] << 8) | (msg->data[3]);
		*config_value = (msg->data[4] << 8) | (msg->data[5]);
		return true;
	} else {
		return false;
	}
}
