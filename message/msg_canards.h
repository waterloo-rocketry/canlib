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
 * @brief Builds a canard module error message
 *
 * @param prio Message priority
 * @param timestamp Message timestamp in milliseconds
 * @param module_id Module ID
 * @param error_bitfield Error code bitfield
 * @param severity Error severity
 * @param output Output message buffer
 */
void build_canard_firmware_error_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t module_id,
									 uint32_t error_bitfield, uint8_t severity, can_msg_t *output);

/*
 * @brief Decode a canard firmware error message
 *
 * @param msg Input message buffer
 * @param module_id Module ID
 * @param error_bitfield Error code bitfield
 * @param severity Error severity
 *
 * @return W_SUCCESS if message parsed successfully, W_INVALID_PARAM if message type is not
 * MSG_CANARD_FIRMWARE_ERROR, W_DATA_FORMAT_ERROR if message data length is not 7
 */
w_status_t get_canard_firmware_error_msg(const can_msg_t *msg, uint8_t *module_id,
										 uint32_t *error_bitfield, uint8_t *severity);

#ifdef __cplusplus
}
#endif

#endif
