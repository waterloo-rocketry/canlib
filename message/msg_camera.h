#ifndef CANLIB_CAMERA_H
#define CANLIB_CAMERA_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CAMERA_SNAPSHOT_CHUNK_BYTES 5U

typedef enum {
    CAMERA_SNAPSHOT_STATUS_OK = 0,
    CAMERA_SNAPSHOT_STATUS_BUSY = 1,
    CAMERA_SNAPSHOT_STATUS_CAMERA_OFF = 2,
    CAMERA_SNAPSHOT_STATUS_CAPTURE_FAILED = 3,
    CAMERA_SNAPSHOT_STATUS_NO_FRAME = 4,
} camera_snapshot_status_t;

bool build_camera_snapshot_status_msg(
    can_msg_prio_t prio, uint16_t timestamp, uint8_t frame_id, uint8_t request_id,
    camera_snapshot_status_t status, uint32_t total_size, can_msg_t *output
);

bool build_camera_snapshot_chunk_msg(
    can_msg_prio_t prio, uint8_t frame_id, uint16_t chunk_idx, bool is_last,
    const uint8_t *payload, uint8_t payload_len, can_msg_t *output
);

bool parse_camera_snapshot_cmd(const can_msg_t *msg, uint8_t *request_id);

#ifdef __cplusplus
}
#endif

#endif
