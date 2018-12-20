#include "can_common_tests.h"
#include "can_common.h"
#include "message_types.h"

#include <stdio.h>
#include <string.h>

static bool test_get_message_type(void) {
    can_msg_t output;
    uint8_t input_data[8];
    // for each type of message, generate a message of that type, then
    // check that get_message_type returns that. I'm lazy, I don't
    // want to type all that, so here comes a sketchy macro
#define CHECK_GET_MESSAGE_TYPE(x)                                       \
    do {                                                                \
        if (!build_can_message((x), 0, input_data, &output)) {          \
            printf("fail on build_can_message when msg_type = 0x%x\n", (x)); \
            return false;                                               \
        }                                                               \
        if ((x) != get_message_type(&output)) {                         \
            printf("fail on get_message_type when msg_type = 0x%x\n", (x)); \
            return false;                                               \
        }                                                               \
    } while(0)

    CHECK_GET_MESSAGE_TYPE(MSG_GENERAL_CMD);
    CHECK_GET_MESSAGE_TYPE(MSG_VENT_VALVE_CMD);
    CHECK_GET_MESSAGE_TYPE(MSG_INJ_VALVE_CMD);
    CHECK_GET_MESSAGE_TYPE(MSG_DEBUG_MSG);
    CHECK_GET_MESSAGE_TYPE(MSG_DEBUG_PRINTF);
    CHECK_GET_MESSAGE_TYPE(MSG_VENT_VALVE_STATUS);
    CHECK_GET_MESSAGE_TYPE(MSG_INJ_VALVE_STATUS);
    CHECK_GET_MESSAGE_TYPE(MSG_GENERAL_BOARD_STATUS);
    CHECK_GET_MESSAGE_TYPE(MSG_SENSOR_ACC);
    CHECK_GET_MESSAGE_TYPE(MSG_SENSOR_GYRO);
    CHECK_GET_MESSAGE_TYPE(MSG_SENSOR_MAG);
    CHECK_GET_MESSAGE_TYPE(MSG_SENSOR_ANALOG);
    CHECK_GET_MESSAGE_TYPE(MSG_LEDS_ON);
    CHECK_GET_MESSAGE_TYPE(MSG_LEDS_OFF);

    return true;
}

static bool test_is_sensor_data(void) {
    can_msg_t output;
    uint8_t input_data[8];
#define CHECK_IS_NOT_SENSOR_DATA(x)                                     \
    do {                                                                \
        if (!build_can_message((x), 0, input_data, &output)) {          \
            printf("fail on build_can_message when msg_type = 0x%x\n", (x)); \
            return false;                                               \
        }                                                               \
        if (is_sensor_data(&output)) {                                  \
            printf("fail on is_sensor_data when msg_type = 0x%x\n", (x)); \
            return false;                                               \
        }                                                               \
    } while(0)
#define CHECK_IS_SENSOR_DATA(x)                                         \
    do {                                                                \
        if (!build_can_message((x), 0, input_data, &output)) {          \
            printf("fail on build_can_message when msg_type = 0x%x\n", (x)); \
            return false;                                               \
        }                                                               \
        if (!is_sensor_data(&output)) {                                 \
            printf("fail on is_sensor_data when msg_type = 0x%x\n", (x)); \
            return false;                                               \
        }                                                               \
    } while(0)

    CHECK_IS_NOT_SENSOR_DATA(MSG_GENERAL_CMD);
    CHECK_IS_NOT_SENSOR_DATA(MSG_VENT_VALVE_CMD);
    CHECK_IS_NOT_SENSOR_DATA(MSG_INJ_VALVE_CMD);
    CHECK_IS_NOT_SENSOR_DATA(MSG_DEBUG_MSG);
    CHECK_IS_NOT_SENSOR_DATA(MSG_DEBUG_PRINTF);
    CHECK_IS_NOT_SENSOR_DATA(MSG_VENT_VALVE_STATUS);
    CHECK_IS_NOT_SENSOR_DATA(MSG_INJ_VALVE_STATUS);
    CHECK_IS_NOT_SENSOR_DATA(MSG_GENERAL_BOARD_STATUS);
    CHECK_IS_NOT_SENSOR_DATA(MSG_LEDS_ON);
    CHECK_IS_NOT_SENSOR_DATA(MSG_LEDS_OFF);

    CHECK_IS_SENSOR_DATA(MSG_SENSOR_ACC);
    CHECK_IS_SENSOR_DATA(MSG_SENSOR_GYRO);
    CHECK_IS_SENSOR_DATA(MSG_SENSOR_MAG);
    CHECK_IS_SENSOR_DATA(MSG_SENSOR_ANALOG);

    return true;
}

static bool test_debug_level_message(void) {
    can_msg_t output;
    uint8_t input_data[8];
#define CHECK_IS_NOT_DEBUG_MSG(x)                                       \
    do {                                                                \
        if (!build_can_message((x), 0, input_data, &output)) {          \
            printf("fail on build_can_message when msg_type = 0x%x\n", (x)); \
            return false;                                               \
        }                                                               \
        if (NONE != debug_level_message(&output)) {                             \
            printf("fail on debug_level_message when msg_type = 0x%x\n", (x)); \
            return false;                                               \
        }                                                               \
    } while(0)

    CHECK_IS_NOT_DEBUG_MSG(MSG_GENERAL_CMD);
    CHECK_IS_NOT_DEBUG_MSG(MSG_VENT_VALVE_CMD);
    CHECK_IS_NOT_DEBUG_MSG(MSG_INJ_VALVE_CMD);
    CHECK_IS_NOT_DEBUG_MSG(MSG_DEBUG_PRINTF);
    CHECK_IS_NOT_DEBUG_MSG(MSG_VENT_VALVE_STATUS);
    CHECK_IS_NOT_DEBUG_MSG(MSG_INJ_VALVE_STATUS);
    CHECK_IS_NOT_DEBUG_MSG(MSG_GENERAL_BOARD_STATUS);
    CHECK_IS_NOT_DEBUG_MSG(MSG_SENSOR_ACC);
    CHECK_IS_NOT_DEBUG_MSG(MSG_SENSOR_GYRO);
    CHECK_IS_NOT_DEBUG_MSG(MSG_SENSOR_MAG);
    CHECK_IS_NOT_DEBUG_MSG(MSG_SENSOR_ANALOG);
    CHECK_IS_NOT_DEBUG_MSG(MSG_LEDS_ON);
    CHECK_IS_NOT_DEBUG_MSG(MSG_LEDS_OFF);

    // Check for ERROR
    input_data[0] = 0x10;
    if(!build_can_message(MSG_DEBUG_MSG, 0, input_data, &output)) {
        printf("fail on build_can_message when level is ERROR\n");
        return false;
    }
    if (ERROR != debug_level_message(&output)) {
        printf("fail on debug_level_message when level is ERROR\n");
        return false;
    }
    // Check for WARN
    input_data[0] = 0x20;
    if(!build_can_message(MSG_DEBUG_MSG, 0, input_data, &output)) {
        printf("fail on build_can_message when level is WARN\n");
        return false;
    }
    if (WARN != debug_level_message(&output)) {
        printf("fail on debug_level_message when level is WARN\n");
        return false;
    }
    // Check for INFO
    input_data[0] = 0x30;
    if(!build_can_message(MSG_DEBUG_MSG, 0, input_data, &output)) {
        printf("fail on build_can_message when level is INFO\n");
        return false;
    }
    if (INFO != debug_level_message(&output)) {
        printf("fail on debug_level_message when level is INFO\n");
        return false;
    }
    // Check for DEBUG
    input_data[0] = 0x40;
    if(!build_can_message(MSG_DEBUG_MSG, 0, input_data, &output)) {
        printf("fail on build_can_message when level is DEBUG\n");
        return false;
    }
    if (DEBUG != debug_level_message(&output)) {
        printf("fail on debug_level_message when level is DEBUG\n");
        return false;
    }
    return true;
}

static bool test_debug_printf(void) {
    // test that debug printf works, and can write out "does this work lol?" (which should take 3 messages to write)
    can_msg_t output;
    const char *message = "does this work lol?";

    // this call should put "does thi" in output, and should set message to "s work lol?"
    message = build_printf_can_message(message, &output);
    if(output.data_len != 8) {
        printf("First call to build_printf_can_message didn't set data_len properly\n");
        return false;
    } else if (output.data[0] != 'd' ||
               output.data[1] != 'o' ||
               output.data[2] != 'e' ||
               output.data[3] != 's' ||
               output.data[4] != ' ' ||
               output.data[5] != 't' ||
               output.data[6] != 'h' ||
               output.data[7] != 'i') {
        printf("First call to build_printf_can_message didn't set data properly\n");
        return false;
    } else if (strcmp(message, "s work lol?")) {
        printf("First call to build_printf_can_message didn't set message properly\n");
        return false;
    } else if (output.sid != 0x1E3) {
        printf("First call to build_printf_can_message didn't set sid properly\n");
        return false;
    }

    // this call should put "s work l" in output, and should set message to "ol?"
    message = build_printf_can_message(message, &output);
    if(output.data_len != 8) {
        printf("Second call to build_printf_can_message didn't set data_len properly\n");
        return false;
    } else if (output.data[0] != 's' ||
               output.data[1] != ' ' ||
               output.data[2] != 'w' ||
               output.data[3] != 'o' ||
               output.data[4] != 'r' ||
               output.data[5] != 'k' ||
               output.data[6] != ' ' ||
               output.data[7] != 'l') {
        printf("Second call to build_printf_can_message didn't set data properly\n");
        return false;
    } else if (strcmp(message, "ol?")) {
        printf("Second call to build_printf_can_message didn't set message properly\n");
        return false;
    } else if (output.sid != 0x1E3) {
        printf("Second call to build_printf_can_message didn't set sid properly\n");
        return false;
    }

    // this call should put "ol?" in output, and should set message to '\0'
    message = build_printf_can_message(message, &output);
    if(output.data_len != 3) {
        printf("Third call to build_printf_can_message didn't set data_len properly\n");
        return false;
    } else if (output.data[0] != 'o' ||
               output.data[1] != 'l' ||
               output.data[2] != '?') {
        printf("Third call to build_printf_can_message didn't set data properly\n");
        return false;
    } else if (*message != '\0') {
        printf("Third call to build_printf_can_message didn't set message properly\n");
        return false;
    } else if (output.sid != 0x1E3) {
        printf("Third call to build_printf_can_message didn't set sid properly\n");
        return false;
    }

    // this call should put nothing in output, and shouldn't change message
    message = build_printf_can_message(message, &output);
    if(output.data_len != 0) {
        printf("Fourth call to build_printf_can_message didn't set data_len properly\n");
        return false;
    } else if (*message != '\0') {
        printf("Fourth call to build_printf_can_message didn't set message properly\n");
        return false;
    } else if (output.sid != 0x1E3) {
        printf("Fourth call to build_printf_can_message didn't set sid properly\n");
        return false;
    }


    return true;
}

bool test_can_common_functions(void) {
    if(!test_get_message_type()) {
        printf(__FILE__ ": Error, test_get_message_type returned false\n");
        return false;
    } else if(!test_is_sensor_data()) {
        printf(__FILE__ ": Error, test_is_sensor_data returned false\n");
        return false;
    } else if(!test_debug_level_message()) {
        printf(__FILE__ ": Error, test_debug_level_message returned false\n");
        return false;
    } else if(!test_debug_printf()) {
        printf(__FILE__ ": Error, test_debug_printf returned false\n");
        return false;
    }

    return true;
}
