#ifndef CANLIB_GENERAL_H
#define CANLIB_GENERAL_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

/*
 * Debug levels for the debugging messages (MSG_DEBUG_MSG). Lower
 * numbers are more serious debug things
 */
typedef enum {
    LVL_NONE = 0,
    LVL_ERROR = 1,
    LVL_WARN = 2,
    LVL_INFO = 3,
    LVL_DEBUG = 4
} can_debug_level_t;

/*
 * This macro creates a new debug message, and stores it in
 * debug_macro_output. The reason that this is a macro and not a
 * function is that debug messages have the line number at which they
 * are created embedded in their data. This is so that we can review
 * the code later to see where the debug was issued from, and
 * hopefully find the cause of the problem
 */
#define LOG_MSG(debug_macro_level, debug_macro_timestamp, debug_macro_output)                      \
    do {                                                                                           \
        uint8_t debug_macro_data[5] = {                                                            \
            (debug_macro_level << 4) | ((__LINE__ >> 8) & 0xF), __LINE__ & 0xFF, 0, 0, 0           \
        };                                                                                         \
        build_debug_msg(debug_macro_timestamp, debug_macro_data, &debug_macro_output);             \
    } while (0)

/*
 * Used for general system-wide commands. Command types are defined in
 * message_types.h
 */
bool build_general_cmd_msg(
    can_msg_prio_t prio, uint32_t timestamp, enum GEN_CMD cmd, can_msg_t *output
);

/*
 * Used by each board to send status messages. Error codes and their
 * corresponding supplemental data are defined in message_types.h.
 * This function may need to be modified to better hide the internals.
 */
bool build_board_stat_msg(
    can_msg_prio_t prio, uint32_t timestamp, enum BOARD_STATUS error_code,
    const uint8_t *error_data, uint8_t error_data_len, can_msg_t *output
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
 * Gets the general command contained in a general command message.
 * Returns -1 if the provided message is not a general cmd message.
 */
int get_general_cmd_type(const can_msg_t *msg);

/*
 * Gets the board ID of the board to be reset
 * Returns false if the provided message is not a reset command message.
 */
bool get_reset_board_id(const can_msg_t *msg, uint8_t *board_type_id, uint8_t *board_inst_id);

/*
 * If MSG is a DEBUG_MSG message, return its debug level, else return
 * NONE
 */
can_debug_level_t message_debug_level(const can_msg_t *msg);

/*
 * These commands build CAN messages whose data bytes are the ASCII characters
 * from string. The functions return a pointer to the first character in string
 * which wasn't copied into output.
 *
 * So if you pass the function string="a long string", "a long s" will be put into
 * output, and "tring" will be returned. You can tell if all of the string was
 * copied into output if the return value points to a '\0'
 *
 * You can build and send an arbitrary length string with the following C code:
 *   const char* string = "arbitrarily long string";
 *   can_msg_t output;
 *   while (*string) {
 *       string = build_printf_can_message(string, &output);
 *       can_send(&output);
 *   }
 */
const char *build_printf_can_message(can_msg_prio_t prio, const char *string, can_msg_t *output);
const char *build_radio_cmd_can_message(can_msg_prio_t prio, const char *string, can_msg_t *output);

#endif
