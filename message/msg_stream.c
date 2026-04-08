#include <stdint.h>
#include <string.h>

#include "common.h"

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_stream.h"

// Fields are 24-bit big-endian.
#define STREAM_SIZE_MAX 0xFFFFFFu

void build_stream_status_msg(can_msg_prio_t prio, uint16_t timestamp, uint32_t total_size,
							 uint32_t tx_size, can_msg_t *output) {
	w_assert(output);
	w_assert(total_size <= STREAM_SIZE_MAX);
	w_assert(tx_size <= STREAM_SIZE_MAX);

	output->sid = build_sid(prio, MSG_STREAM_STATUS, 0);
	write_timestamp(timestamp, output);

	output->data[2] = (uint8_t)((total_size >> 16) & 0xFF);
	output->data[3] = (uint8_t)((total_size >> 8) & 0xFF);
	output->data[4] = (uint8_t)(total_size & 0xFF);
	output->data[5] = (uint8_t)((tx_size >> 16) & 0xFF);
	output->data[6] = (uint8_t)((tx_size >> 8) & 0xFF);
	output->data[7] = (uint8_t)(tx_size & 0xFF);
	output->data_len = 8;
}

void build_stream_data_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t seq_id,
						   const uint8_t *payload, uint8_t payload_len, can_msg_t *output) {
	w_assert(output);
	w_assert(payload);
	w_assert(payload_len <= STREAM_DATA_MAX_PAYLOAD_LEN);

	output->sid = build_sid(prio, MSG_STREAM_DATA, seq_id);
	write_timestamp(timestamp, output);

	memcpy(&output->data[2], payload, payload_len);

	output->data_len = (uint8_t)(payload_len + 2);
}

void build_stream_retry_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t seq_id,
							can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_STREAM_RETRY, seq_id);
	write_timestamp(timestamp, output);

	output->data_len = 2;
}

bool get_stream_status(const can_msg_t *msg, uint32_t *total_size, uint32_t *tx_size) {
	w_assert(msg);
	w_assert(total_size);
	w_assert(tx_size);

	if (get_message_type(msg) != MSG_STREAM_STATUS || msg->data_len != 8) {
		return false;
	}

	*total_size = ((uint32_t)msg->data[2] << 16) | ((uint32_t)msg->data[3] << 8) | msg->data[4];
	*tx_size = ((uint32_t)msg->data[5] << 16) | ((uint32_t)msg->data[6] << 8) | msg->data[7];

	return true;
}

bool get_stream_data(const can_msg_t *msg, uint8_t *seq_id, uint8_t *payload,
					 uint8_t *payload_len) {
	w_assert(msg);
	w_assert(seq_id);
	w_assert(payload);
	w_assert(payload_len);

	if ((get_message_type(msg) != MSG_STREAM_DATA) || (msg->data_len < 4) || (msg->data_len > 8)) {
		return false;
	}

	uint8_t msg_payload_len = (uint8_t)(msg->data_len - 2);
	memcpy(payload, &msg->data[2], msg_payload_len);

	*seq_id = msg->sid & 0xff;
	*payload_len = msg_payload_len;

	return true;
}

bool get_stream_retry_seq_id(const can_msg_t *msg, uint8_t *seq_id) {
	w_assert(msg);
	w_assert(seq_id);

	if (get_message_type(msg) != MSG_STREAM_RETRY || msg->data_len != 2) {
		return false;
	}

	*seq_id = get_message_metadata(msg);

	return true;
}
