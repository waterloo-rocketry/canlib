#include <string.h>

#include "msg_camera.h"

bool build_camera_snapshot_status_msg(
    can_msg_prio_t prio, uint16_t timestamp, uint8_t frame_id, uint8_t request_id,
    camera_snapshot_status_t status, uint32_t total_size, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_CAMERA_SNAPSHOT_STATUS);
    write_timestamp_2bytes(timestamp, output);
    output->data[2] = frame_id;
    output->data[3] = request_id;
    output->data[4] = (uint8_t)status;
    output->data[5] = (total_size >> 16) & 0xFF;
    output->data[6] = (total_size >> 8) & 0xFF;
    output->data[7] = total_size & 0xFF;
    output->data_len = 8;

    return true;
}

bool build_camera_snapshot_chunk_msg(
    can_msg_prio_t prio, uint8_t frame_id, uint16_t chunk_idx, bool is_last,
    const uint8_t *payload, uint8_t payload_len, can_msg_t *output
) {
    if (!output || !payload) {
        return false;
    }

    if (payload_len == 0 || payload_len > CAMERA_SNAPSHOT_CHUNK_BYTES) {
        return false;
    }

    output->sid = SID(prio, MSG_CAMERA_SNAPSHOT_CHUNK);
    output->data[0] = frame_id;
    uint16_t chunk = chunk_idx & 0x7FFF;
    output->data[1] = (chunk >> 8) & 0x7F;
    if (is_last) {
        output->data[1] |= 0x80;
    }
    output->data[2] = chunk & 0xFF;
    memcpy(output->data + 3, payload, payload_len);
    output->data_len = (uint8_t)(payload_len + 3);

    return true;
}

bool parse_camera_snapshot_cmd(const can_msg_t *msg, uint8_t *request_id) {
    if (!msg) {
        return false;
    }

    if (get_message_type(msg) != MSG_CAMERA_SNAPSHOT_CMD) {
        return false;
    }

    if (msg->data_len < 3) {
        return false;
    }

    if (request_id) {
        *request_id = msg->data[2];
    }

    return true;
}
