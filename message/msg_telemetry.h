#ifndef CANLIB_TELEMETRY_H
#define CANLIB_TELEMETRY_H

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

#include "can.h"
#include "message_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Builds a telemetry info message
 *
 * @param prio Message priority
 * @param timestamp Message timestamp in milliseconds
 * @param channel_id Channel ID (Use LTT board instance ID)
 * @param lqi Link Quality Indicator
 * @param rssi Received Signal Strength Indicator
 * @param output Output message buffer
 */
void build_telemetry_info_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t channel_id,
							  uint8_t lqi, uint8_t rssi, can_msg_t *output);

/*
 * @brief Decode a telemetry info message
 *
 * @param msg Input message buffer
 * @param module_id Channel ID (Use LTT board instance ID)
 * @param lqi Link Quality Indicator
 * @param rssi Received Signal Strength Indicator
 *
 * @return W_SUCCESS if message parsed successfully, W_INVALID_PARAM if message type is not
 * MSG_TELEMETRY_INFO, W_DATA_FORMAT_ERROR if message data length is not 4
 */
w_status_t get_telemetry_info_msg(const can_msg_t *msg, uint8_t *channel_id, uint8_t *lqi,
								  uint8_t *rssi);

/*
 * @brief Builds a telemetry state switch message
 *
 * @param prio Message priority
 * @param timestamp Message timestamp in milliseconds
 * @param channel_id Target channel ID (Use LTT board instance ID)
 * @param output Output message buffer
 */
void build_telemetry_state_switch_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t channel_id,
									  can_msg_t *output);

/*
 * @brief Decode a telemetry info message
 *
 * @param msg Input message buffer
 * @param module_id Target channel ID (Use LTT board instance ID)
 *
 * @return W_SUCCESS if message parsed successfully, W_INVALID_PARAM if message type is not
 * MSG_TELEMETRY_STATE_SWITCH, W_DATA_FORMAT_ERROR if message data length is not 4
 */
w_status_t get_telemetry_state_switch_msg(const can_msg_t *msg, uint8_t *channel_id);

#ifdef __cplusplus
}
#endif

#endif
