#include <stdbool.h>
#include <stdint.h>

#include "common.h"

#include "can.h"
#include "message_types.h"
#include "msg_canards.h"
#include "msg_common.h"

void build_canard_firmware_error_msg(can_msg_prio_t prio, uint16_t timestamp,
									 can_canards_module_id_t module_id, uint32_t error_bitfield,
									 can_canards_health_severity_t severity, can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_CANARD_FIRMWARE_ERROR, module_id);
	write_timestamp(timestamp, output);

	output->data[2] = (error_bitfield >> 24) & 0xff;
	output->data[3] = (error_bitfield >> 16) & 0xff;
	output->data[4] = (error_bitfield >> 8) & 0xff;
	output->data[5] = error_bitfield & 0xff;
	output->data[6] = (uint8_t)severity;
	output->data_len = 7;
}

w_status_t get_canard_firmware_error_msg(const can_msg_t *msg, can_canards_module_id_t *module_id,
										 uint32_t *error_bitfield,
										 can_canards_health_severity_t *severity) {
	w_assert(msg);
	w_assert(module_id);
	w_assert(error_bitfield);
	w_assert(severity);

	*module_id = (can_canards_module_id_t)get_message_metadata(msg);
	*error_bitfield = ((uint32_t)msg->data[2] << 24) | ((uint32_t)msg->data[3] << 16) |
					  ((uint32_t)msg->data[4] << 8) | (uint32_t)msg->data[5];
	*severity = (can_canards_health_severity_t)msg->data[6];

	if (get_message_type(msg) != MSG_CANARD_FIRMWARE_ERROR) {
		return W_INVALID_PARAM;
	}

	if (msg->data_len != 7) {
		return W_DATA_FORMAT_ERROR;
	}

	return W_SUCCESS;
}
