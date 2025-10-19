#ifndef CANLIB_STREAM_H
#define CANLIB_STREAM_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Max payload bytes that fit in a classic CAN frame once timestamp and seqId are included.
#define STREAM_DATA_MAX_PAYLOAD_LEN 5

bool build_stream_status_msg(can_msg_prio_t prio, uint16_t timestamp, uint32_t total_size,
                             uint32_t tx_size, can_msg_t *output);

bool build_stream_data_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t seq_id,
                           const uint8_t *payload, uint8_t payload_len, can_msg_t *output);

bool build_stream_retry_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t seq_id,
                            can_msg_t *output);

bool get_stream_status(const can_msg_t *msg, uint32_t *total_size, uint32_t *tx_size);

bool get_stream_data(const can_msg_t *msg, uint8_t *seq_id, uint8_t *payload,
                     uint8_t payload_buf_len, uint8_t *payload_len);

bool get_stream_retry_seq_id(const can_msg_t *msg, uint8_t *seq_id);

#ifdef __cplusplus
}
#endif

#endif
