#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_general.h"

bool build_general_cmd_msg(
    can_msg_prio_t prio, uint32_t timestamp, enum GEN_CMD cmd, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_GENERAL_CMD);
    write_timestamp_3bytes(timestamp, output);
    output->data[3] = (uint8_t)cmd;
    output->data_len = 4; // 3 bytes timestamp, 1 byte data

    return true;
}

bool build_board_stat_msg(
    can_msg_prio_t prio, uint32_t timestamp, enum BOARD_STATUS error_code,
    const uint8_t *error_data, uint8_t error_data_len, can_msg_t *output
) {
    if (error_data_len > 0 && !error_data) {
        return false;
    }
    if (!output) {
        return false;
    }
    if (error_data_len > 4) {
        return false;
    }

    output->sid = SID(prio, MSG_GENERAL_BOARD_STATUS);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = (uint8_t)error_code;
    for (int i = 0; i < error_data_len; ++i) {
        // error data goes in message bytes 4-7
        output->data[4 + i] = error_data[i];
    }

    // ugly but: 3 bytes timestamp, 1 byte error code, x bytes data
    output->data_len = 4 + error_data_len;

    return true;
}

bool build_debug_msg(
    can_msg_prio_t prio, uint32_t timestamp, const uint8_t *debug_data, can_msg_t *output
) {
    if (!output) {
        return false;
    }
    if (!debug_data) {
        return false;
    }

    output->sid = SID(prio, MSG_DEBUG_MSG);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = debug_data[0];
    output->data[4] = debug_data[1];
    output->data[5] = debug_data[2];
    output->data[6] = debug_data[3];
    output->data[7] = debug_data[4];

    output->data_len = 8;
    return true;
}

bool build_debug_printf(can_msg_prio_t prio, const uint8_t *input_data, can_msg_t *output) {
    if (!output) {
        return false;
    }
    if (!input_data) {
        return false;
    }

    output->sid = SID(prio, MSG_DEBUG_PRINTF);
    for (int i = 0; i < 8; ++i) {
        output->data[i] = input_data[i];
    }
    output->data_len = 8;
    return true;
}

bool build_reset_msg(
    can_msg_prio_t prio, uint32_t timestamp, uint8_t board_type_id, uint8_t board_inst_id,
    can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_RESET_CMD);
    write_timestamp_3bytes(timestamp, output);
    output->data[3] = board_type_id;
    output->data[4] = board_inst_id;
    output->data_len = 5; // 3 bytes timestamp, 2 byte data

    return true;
}

// This might need to be made more granular - it doesn't quite hide
// the data layout properly.

int get_general_cmd_type(const can_msg_t *msg) {
    if (!msg) {
        return -1;
    }

    uint16_t msg_type = get_message_type(msg);
    if (msg_type == MSG_GENERAL_CMD) {
        return msg->data[3];
    } else {
        return -1;
    }
}

bool get_reset_board_id(const can_msg_t *msg, uint8_t *board_type_id, uint8_t *board_inst_id) {
    if (!msg) {
        return false;
    }

    uint16_t msg_type = get_message_type(msg);
    if (msg_type == MSG_RESET_CMD) {
        *board_type_id = msg->data[3];
        *board_inst_id = msg->data[4];
        return true;
    } else {
        // not a valid field for this message type
        return false;
    }
}

can_debug_level_t message_debug_level(const can_msg_t *msg) {
    uint16_t type = get_message_type(msg);
    if (type != MSG_DEBUG_MSG) {
        return LVL_NONE;
    } else {
        // As per the spreadsheet, the debug level of a DEBUG_MSG is
        // stored in the top nibble of the 4th data byte (yeah, I
        // know, I'm sorry). To get at it, you right shift the 4th
        // data byte by 4. bitwise anding with 0xf probably isn't
        // necessary, but it ensures that the returned data is no more
        // than 4 bits.
        return ((msg->data[3] >> 4) & 0xf);
    }
}

/* In the grand tradition of C programmers, string handling funcitons
 * must be as convoluted as possible.  If you call this function with
 * a string that is longer than 8 bytes, it's impossible to fit all of
 * that into one CAN message. So what this function does is it puts
 * the first 8 bytes of the string into the CAN message output, then
 * returns a pointer that's 8 bytes ahead of the string (string +
 * 8). If it's called with less than 8 characters of data, it returns
 * a pointer to '\0' (the null terminator in string). This lets you
 * write code like the following:
 *
 * const char *string = "a really long message to be sent on CAN";
 * can_msg_t to_send;
 * while (*string) {
 *     string = build_printf_can_message(string, &to_send);
 *     can_send(&to_send);
 * }
 */
const char *build_printf_can_message(can_msg_prio_t prio, const char *string, can_msg_t *output) {
    // set the SID of ouput
    output->sid = SID(prio, MSG_DEBUG_PRINTF);
    uint8_t i;
    for (i = 0; i < 8; ++i) {
        if (*string == '\0') {
            output->data_len = i;
            return string;
        }
        output->data[i] = *string;
        string++;
    }
    output->data_len = i;
    return string;
}

const char *
build_radio_cmd_can_message(can_msg_prio_t prio, const char *string, can_msg_t *output) {
    // let build_printf_can_message do all the heavy lifting
    string = build_printf_can_message(prio, string, output);
    // then just set SID, since that's the only difference in message type
    output->sid = SID(prio, MSG_DEBUG_RADIO_CMD);
    return string;
}
