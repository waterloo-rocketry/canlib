#ifndef CANLIB_CANARDS_H
#define CANLIB_CANARDS_H

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

#include "can.h"
#include "message_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Builds a canard module status message. module_id is encoded in the SID
 * metadata field.
 */
void build_canard_firmware_error_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t module_id,
									 uint32_t error_bitfield, uint8_t severity, can_msg_t *output);

w_status_t get_canard_firmware_error_msg(const can_msg_t *msg, uint8_t *module_id,
										 uint32_t *error_bitfield, uint8_t *severity);

#ifdef __cplusplus
}
#endif

#endif
