#include "can_common.h"
#include "message_types.h"
#include <stddef.h>

// this symbol should be defined in the project's Makefile, but if it
// isn't, issue a warning and set it to 0
#ifndef  BOARD_UNIQUE_ID
#warning BOARD_UNIQUE_ID not defined, please set that up in project
#define  BOARD_UNIQUE_ID 0
#endif

bool build_can_message(uint16_t message_type,
                       uint32_t timestamp,
                       const uint8_t *input_data,
                       can_msg_t *output) {
    if(input_data == NULL || output == NULL) {
        // illegal argument results in false return value
        return false;
    }

    // consult spreadsheet linked in message_types.h to see where
    // these values come from
    switch(message_type) {
    case MSG_LEDS_ON:
    case MSG_LEDS_OFF:
        // these two message types have no data in them, so can be
        // handled the same
        output->data_len = 0;
        break;

    case MSG_GENERAL_CMD:
    case MSG_VENT_VALVE_CMD:
    case MSG_INJ_VALVE_CMD:
    case MSG_VENT_VALVE_STATUS:
    case MSG_INJ_VALVE_STATUS:
        // these five message types all have three bytes of timestamp,
        // then a single byte of data, and thus can be handled the same
        output->data_len = 4;
        // first byte of data is highest nibble of timestamp
        output->data[0] = ((timestamp >> 16) & 0xFF);
        output->data[1] = ((timestamp >> 8)  & 0xFF);
        output->data[2] = ((timestamp >> 0)  & 0xFF);
        // copy over the one byte of data that this command takes
        output->data[3] = input_data[0];
        break;

    case MSG_SENSOR_ACC:
    case MSG_SENSOR_GYRO:
    case MSG_SENSOR_MAG:
        // these three message types all have two bytes of timestamp,
        // then six bytes of data
        output->data_len = 8;
        output->data[0] = ((timestamp >> 8) & 0xFF);
        output->data[1] = ((timestamp >> 0) & 0xFF);
        // copy six bytes of data from input_data
        output->data[2] = input_data[0];
        output->data[3] = input_data[1];
        output->data[4] = input_data[2];
        output->data[5] = input_data[3];
        output->data[6] = input_data[4];
        output->data[7] = input_data[5];
        break;

    case MSG_SENSOR_ANALOG:
        // this message type has two bytes of timestamp, then two
        // bytes of data
        output->data_len = 4;
        output->data[0] = ((timestamp >> 8) & 0xFF);
        output->data[1] = ((timestamp >> 0) & 0xFF);
        output->data[2] = input_data[0];
        output->data[3] = input_data[1];
        break;

    case MSG_DEBUG_MSG:
    case MSG_GENERAL_BOARD_STATUS:
        // these two message types have three bytes of timestamp, then
        // 5 bytes of data
        output->data_len = 8;
        output->data[0] = ((timestamp >> 16) & 0xFF);
        output->data[1] = ((timestamp >> 8)  & 0xFF);
        output->data[2] = ((timestamp >> 0)  & 0xFF);
        output->data[3] = input_data[0];
        output->data[4] = input_data[1];
        output->data[5] = input_data[2];
        output->data[6] = input_data[3];
        output->data[7] = input_data[4];
        break;

    case MSG_DEBUG_PRINTF:
        // this message type just has eight bytes of data
        output->data_len = 8;
        output->data[0] = input_data[0];
        output->data[1] = input_data[1];
        output->data[2] = input_data[2];
        output->data[3] = input_data[3];
        output->data[4] = input_data[4];
        output->data[5] = input_data[5];
        output->data[6] = input_data[6];
        output->data[7] = input_data[7];
        break;

    default:
        // message is not one of the types defined in message_types.h,
        // so we should return false (illegal argument)
        return false;
    }

    // set output's SID. By our team's convention, a message's SID is
    // the message type bitwise OR'd with the board's unique ID
    output->sid = (message_type | BOARD_UNIQUE_ID);

    // we've set the data_len, and we've set the SID, and we've set
    // all the data we need to set. We're done
    return true;
}
