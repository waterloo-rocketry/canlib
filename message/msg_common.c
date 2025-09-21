#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"

// Helper function for populating CAN messages
void write_timestamp_2bytes(uint16_t timestamp, can_msg_t *output) {
	output->data[0] = (timestamp >> 8) & 0xff;
	output->data[1] = (timestamp >> 0) & 0xff;
}

can_msg_type_t get_message_type(const can_msg_t *msg) {
	if (!msg) {
		return 0;
	}

	return (can_msg_type_t)((msg->sid >> 18) & 0x1FF);
}

uint8_t get_board_type_unique_id(const can_msg_t *msg) {
	if (!msg) {
		return 0;
	}

	return (uint8_t)((msg->sid >> 8) & 0xFF);
}

uint8_t get_board_inst_unique_id(const can_msg_t *msg) {
	if (!msg) {
		return 0;
	}

	return (uint8_t)(msg->sid & 0xFF);
}

uint16_t get_timestamp(const can_msg_t *msg) {
	if (!msg) {
		return 0;
	}

	return ((uint16_t)msg->data[0] << 8) | msg->data[1];
}
