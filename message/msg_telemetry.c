#include <stdbool.h>
#include <stdint.h>

#include "common.h"

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_telemetry.h"

void build_telemetry_info_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t channel_id,
							  uint8_t lqi, uint8_t rssi, can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_TELEMETRY_INFO, channel_id);
	write_timestamp(timestamp, output);

	output->data[2] = lqi;
	output->data[3] = rssi;
	output->data_len = 4;
}

w_status_t get_telemetry_info_msg(const can_msg_t *msg, uint8_t *channel_id, uint8_t *lqi,
								  uint8_t *rssi) {
	w_assert(msg);
	w_assert(channel_id);
	w_assert(lqi);
	w_assert(rssi);

	*channel_id = get_message_metadata(msg);
	*lqi = msg->data[2];
	*rssi = msg->data[3];

	if (get_message_type(msg) != MSG_TELEMETRY_INFO) {
		return W_INVALID_PARAM;
	}

	if (msg->data_len != 4) {
		return W_DATA_FORMAT_ERROR;
	}

	return W_SUCCESS;
}
