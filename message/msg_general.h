#ifndef CANLIB_GENERAL_H
#define CANLIB_GENERAL_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

/*
 * Used by each board to send status messages. Error codes and their
 * corresponding supplemental data are defined in message_types.h.
 * This function may need to be modified to better hide the internals.
 */
bool build_board_stat_msg(
    can_msg_prio_t prio, uint16_t timestamp, uint32_t general_error_bitfield,
    uint16_t board_specific_error_bitfield, can_msg_t *output
);

bool build_debug_msg(
    can_msg_prio_t prio, uint32_t timestamp, const uint8_t *debug_data, can_msg_t *output
);

bool build_debug_printf(can_msg_prio_t prio, const uint8_t *data, can_msg_t *output);

/*
 * Used to Reset a CAN board
 */
bool build_reset_msg(
    can_msg_prio_t prio, uint32_t timestamp, uint8_t board_type_id, uint8_t board_inst_id,
    can_msg_t *output
);

/*
 * Gets the board ID of the board to be reset
 * Returns false if the provided message is not a reset command message.
 */
bool get_reset_board_id(const can_msg_t *msg, uint8_t *board_type_id, uint8_t *board_inst_id);

#endif
