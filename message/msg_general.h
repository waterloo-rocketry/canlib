#ifndef CANLIB_GENERAL_H
#define CANLIB_GENERAL_H

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

#include "can.h"
#include "message_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Used by each board to send status messages. Error codes and their
 * corresponding supplemental data are defined in message_types.h.
 * This function may need to be modified to better hide the internals.
 */
void build_general_board_status_msg(can_msg_prio_t prio, uint16_t timestamp,
									uint32_t board_error_bitfield, can_msg_t *output);

/*
 * Used to Reset a CAN board
 */
void build_reset_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t board_type_id,
					 uint8_t board_inst_id, can_msg_t *output);

/*
 * Copies first 6 bytes of data
 */
void build_debug_raw_msg(can_msg_prio_t prio, uint16_t timestamp, const uint8_t *data,
						 can_msg_t *output);

void build_config_set_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t board_type_id,
						  uint8_t board_inst_id, uint16_t config_id, uint16_t config_value,
						  can_msg_t *output);

void build_config_status_msg(can_msg_prio_t prio, uint16_t timestamp, uint16_t config_id,
							 uint16_t config_value, can_msg_t *output);

w_status_t get_general_board_status(const can_msg_t *msg, uint32_t *board_error_bitfield);

/*
 * Gets the board ID of the board to be reset
 * Returns false if the provided message is not a reset command message.
 */
w_status_t get_reset_board_id(const can_msg_t *msg, uint8_t *board_type_id, uint8_t *board_inst_id);

w_status_t check_board_need_reset(const can_msg_t *msg, bool *board_need_reset);

w_status_t get_debug_raw_data(const can_msg_t *msg, uint8_t *data);

w_status_t get_config_set_target_board(const can_msg_t *msg, uint8_t *board_type_id,
									   uint8_t *board_inst_id);

w_status_t get_config_id_value(const can_msg_t *msg, uint16_t *config_id, uint16_t *config_value);

#ifdef __cplusplus
}
#endif

#endif
