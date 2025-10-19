#include <string.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_stream.h"

// Fields are 24-bit big-endian.
#define STREAM_SIZE_MAX 0xFFFFFFu

bool build_stream_status_msg(can_msg_prio_t prio, uint16_t timestamp, uint32_t total_size,
                             uint32_t tx_size, can_msg_t *output) {
    if (!output) {
        return false;
    }
    if (total_size > STREAM_SIZE_MAX || tx_size > STREAM_SIZE_MAX) {
        return false;
    }

    output->sid = SID(prio, MSG_STREAM_STATUS);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = (uint8_t)((total_size >> 16) & 0xFF);
    output->data[3] = (uint8_t)((total_size >> 8) & 0xFF);
    output->data[4] = (uint8_t)(total_size & 0xFF);
    output->data[5] = (uint8_t)((tx_size >> 16) & 0xFF);
    output->data[6] = (uint8_t)((tx_size >> 8) & 0xFF);
    output->data[7] = (uint8_t)(tx_size & 0xFF);
    output->data_len = 8;

    return true;
}

bool build_stream_data_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t seq_id,
                           const uint8_t *payload, uint8_t payload_len, can_msg_t *output) {
    if (!output) {
        return false;
    }
    if (payload_len > STREAM_DATA_MAX_PAYLOAD_LEN) {
        return false;
    }
    if (payload_len > 0 && !payload) {
        return false;
    }

    output->sid = SID(prio, MSG_STREAM_DATA);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = seq_id;
    if (payload_len > 0) {
        memcpy(&output->data[3], payload, payload_len);
    }

    output->data_len = (uint8_t)(payload_len + 3);

    return true;
}

bool build_stream_retry_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t seq_id,
                            can_msg_t *output) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_STREAM_RETRY);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = seq_id;
    output->data_len = 3;

    return true;
}

bool get_stream_status(const can_msg_t *msg, uint32_t *total_size, uint32_t *tx_size) {
    if (!msg || !total_size || !tx_size) {
        return false;
    }
    if (get_message_type(msg) != MSG_STREAM_STATUS || msg->data_len != 8) {
        return false;
    }

    *total_size = ((uint32_t)msg->data[2] << 16) | ((uint32_t)msg->data[3] << 8) | msg->data[4];
    *tx_size = ((uint32_t)msg->data[5] << 16) | ((uint32_t)msg->data[6] << 8) | msg->data[7];

    return true;
}

bool get_stream_data(const can_msg_t *msg, uint8_t *seq_id, uint8_t *payload,
                     uint8_t payload_buf_len, uint8_t *payload_len) {
    if (!msg || !seq_id || !payload_len) {
        return false;
    }
    if (get_message_type(msg) != MSG_STREAM_DATA || msg->data_len < 3) {
        return false;
    }

    uint8_t msg_payload_len = (uint8_t)(msg->data_len - 3);
    if (msg_payload_len > STREAM_DATA_MAX_PAYLOAD_LEN) {
        return false;
    }
    if (msg_payload_len > 0) {
        if (!payload || payload_buf_len < msg_payload_len) {
            return false;
        }
        memcpy(payload, &msg->data[3], msg_payload_len);
    }

    *seq_id = msg->data[2];
    *payload_len = msg_payload_len;

    return true;
}

bool get_stream_retry_seq_id(const can_msg_t *msg, uint8_t *seq_id) {
    if (!msg || !seq_id) {
        return false;
    }
    if (get_message_type(msg) != MSG_STREAM_RETRY || msg->data_len != 3) {
        return false;
    }

    *seq_id = msg->data[2];

    return true;
}
