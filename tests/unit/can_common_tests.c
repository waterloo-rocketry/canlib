#include "can_common_tests.h"
#include "can_common.h"
#include "message_types.h"

#include <stdio.h>
#include <string.h>

//if this test is running on hardware, don't actually print anything
#ifdef TARGET_LOCAL
#define REPORT_FAIL(x) printf("%s: Fail: %s\n", __FUNCTION__, x)
#else
#define REPORT_FAIL(x)
#endif


static bool test_message_debug_level(void)
{
    can_msg_t output;
    uint8_t input_data[8];
    bool ret = true;

    // Check for ERROR
    input_data[0] = 0x10;
    if (!build_debug_msg(0, input_data, &output)) {
        REPORT_FAIL("fail on build_can_message when level is ERROR");
        ret = false;
    }
    if (ERROR != message_debug_level(&output)) {
        REPORT_FAIL("fail on message_debug_level when level is ERROR");
        ret = false;
    }

    // Check for WARN
    input_data[0] = 0x20;
    if (!build_debug_msg(0, input_data, &output)) {
        REPORT_FAIL("fail on build_can_message when level is WARN");
        ret = false;
    }
    if (WARN != message_debug_level(&output)) {
        REPORT_FAIL("fail on message_debug_level when level is WARN");
        ret = false;
    }
    // Check for INFO
    input_data[0] = 0x30;
    if (!build_debug_msg(0, input_data, &output)) {
        REPORT_FAIL("fail on build_can_message when level is INFO");
        ret = false;
    }
    if (INFO != message_debug_level(&output)) {
        REPORT_FAIL("fail on message_debug_level when level is INFO");
        ret = false;
    }
    // Check for DEBUG
    input_data[0] = 0x40;
    if (!build_debug_msg(0, input_data, &output)) {
        REPORT_FAIL("fail on build_can_message when level is DEBUG");
        ret = false;
    }
    if (DEBUGGING != message_debug_level(&output)) {
        REPORT_FAIL("fail on message_debug_level when level is DEBUG");
        ret = false;
    }
    return ret;
}

static bool test_debug_printf(void)
{
    // test that debug printf works, and can write out "does this work lol?" (which should take 3 messages to write)
    can_msg_t output;
    const char *message = "does this work lol?";
    bool ret = true;

    // this call should put "does thi" in output, and should set message to "s work lol?"
    message = build_printf_can_message(message, &output);
    if (output.data_len != 8) {
        REPORT_FAIL("First call to build_printf_can_message didn't set data_len properly");
        ret = false;
    } else if (output.data[0] != 'd' ||
               output.data[1] != 'o' ||
               output.data[2] != 'e' ||
               output.data[3] != 's' ||
               output.data[4] != ' ' ||
               output.data[5] != 't' ||
               output.data[6] != 'h' ||
               output.data[7] != 'i') {
        REPORT_FAIL("First call to build_printf_can_message didn't set data properly");
        ret = false;
    } else if (strcmp(message, "s work lol?")) {
        REPORT_FAIL("First call to build_printf_can_message didn't set message properly");
        ret = false;
    } else if (output.sid != 0x1E3) {
        REPORT_FAIL("First call to build_printf_can_message didn't set sid properly");
        ret = false;
    }

    // this call should put "s work l" in output, and should set message to "ol?"
    message = build_printf_can_message(message, &output);
    if (output.data_len != 8) {
        REPORT_FAIL("Second call to build_printf_can_message didn't set data_len properly");
        ret = false;
    } else if (output.data[0] != 's' ||
               output.data[1] != ' ' ||
               output.data[2] != 'w' ||
               output.data[3] != 'o' ||
               output.data[4] != 'r' ||
               output.data[5] != 'k' ||
               output.data[6] != ' ' ||
               output.data[7] != 'l') {
        REPORT_FAIL("Second call to build_printf_can_message didn't set data properly");
        ret = false;
    } else if (strcmp(message, "ol?")) {
        REPORT_FAIL("Second call to build_printf_can_message didn't set message properly");
        ret = false;
    } else if (output.sid != 0x1E3) {
        REPORT_FAIL("Second call to build_printf_can_message didn't set sid properly");
        ret = false;
    }

    // this call should put "ol?" in output, and should set message to '\0'
    message = build_printf_can_message(message, &output);
    if (output.data_len != 3) {
        REPORT_FAIL("Third call to build_printf_can_message didn't set data_len properly");
        ret = false;
    } else if (output.data[0] != 'o' ||
               output.data[1] != 'l' ||
               output.data[2] != '?') {
        REPORT_FAIL("Third call to build_printf_can_message didn't set data properly");
        ret = false;
    } else if (*message != '\0') {
        REPORT_FAIL("Third call to build_printf_can_message didn't set message properly");
        ret = false;
    } else if (output.sid != 0x1E3) {
        REPORT_FAIL("Third call to build_printf_can_message didn't set sid properly");
        ret = false;
    }

    // this call should put nothing in output, and shouldn't change message
    message = build_printf_can_message(message, &output);
    if (output.data_len != 0) {
        REPORT_FAIL("Fourth call to build_printf_can_message didn't set data_len properly");
        ret = false;
    } else if (*message != '\0') {
        REPORT_FAIL("Fourth call to build_printf_can_message didn't set message properly");
        ret = false;
    } else if (output.sid != 0x1E3) {
        REPORT_FAIL("Fourth call to build_printf_can_message didn't set sid properly");
        ret = false;
    }

    return ret;
}

static bool test_debug_radio_cmd(void)
{
    // test that debug radio works, and can write out "does this work lol?" (which should take 3 messages to write)
    can_msg_t output;
    const char *message = "does this work lol?";
    bool ret = true;

    // this call should put "does thi" in output, and should set message to "s work lol?"
    message = build_radio_cmd_can_message(message, &output);
    if (output.data_len != 8) {
        REPORT_FAIL("First call to build_radio_cmd_can_message didn't set data_len properly");
        ret = false;
    } else if (output.data[0] != 'd' ||
               output.data[1] != 'o' ||
               output.data[2] != 'e' ||
               output.data[3] != 's' ||
               output.data[4] != ' ' ||
               output.data[5] != 't' ||
               output.data[6] != 'h' ||
               output.data[7] != 'i') {
        REPORT_FAIL("First call to build_radio_cmd_can_message didn't set data properly");
        ret = false;
    } else if (strcmp(message, "s work lol?")) {
        REPORT_FAIL("First call to build_radio_cmd_can_message didn't set message properly");
        ret = false;
    } else if (output.sid != 0x203) {
        REPORT_FAIL("First call to build_radio_cmd_can_message didn't set sid properly");
        ret = false;
    }

    // this call should put "s work l" in output, and should set message to "ol?"
    message = build_radio_cmd_can_message(message, &output);
    if (output.data_len != 8) {
        REPORT_FAIL("Second call to build_radio_cmd_can_message didn't set data_len properly");
        ret = false;
    } else if (output.data[0] != 's' ||
               output.data[1] != ' ' ||
               output.data[2] != 'w' ||
               output.data[3] != 'o' ||
               output.data[4] != 'r' ||
               output.data[5] != 'k' ||
               output.data[6] != ' ' ||
               output.data[7] != 'l') {
        REPORT_FAIL("Second call to build_radio_cmd_can_message didn't set data properly");
        ret = false;
    } else if (strcmp(message, "ol?")) {
        REPORT_FAIL("Second call to build_radio_cmd_can_message didn't set message properly");
        ret = false;
    } else if (output.sid != 0x203) {
        REPORT_FAIL("Second call to build_radio_cmd_can_message didn't set sid properly");
        ret = false;
    }

    // this call should put "ol?" in output, and should set message to '\0'
    message = build_radio_cmd_can_message(message, &output);
    if (output.data_len != 3) {
        REPORT_FAIL("Third call to build_radio_cmd_can_message didn't set data_len properly");
        ret = false;
    } else if (output.data[0] != 'o' ||
               output.data[1] != 'l' ||
               output.data[2] != '?') {
        REPORT_FAIL("Third call to build_radio_cmd_can_message didn't set data properly");
        ret = false;
    } else if (*message != '\0') {
        REPORT_FAIL("Third call to build_radio_cmd_can_message didn't set message properly");
        ret = false;
    } else if (output.sid != 0x203) {
        REPORT_FAIL("Third call to build_radio_cmd_can_message didn't set sid properly");
        ret = false;
    }

    // this call should put nothing in output, and shouldn't change message
    message = build_radio_cmd_can_message(message, &output);
    if (output.data_len != 0) {
        REPORT_FAIL("Fourth call to build_radio_cmd_can_message didn't set data_len properly");
        ret = false;
    } else if (*message != '\0') {
        REPORT_FAIL("Fourth call to build_radio_cmd_can_message didn't set message properly");
        ret = false;
    } else if (output.sid != 0x203) {
        REPORT_FAIL("Fourth call to build_radio_cmd_can_message didn't set sid properly");
        ret = false;
    }

    return ret;
}

bool test_debug_macro(void)
{
    // run the debug macro, and check that it puts in the line number
    // right
    can_msg_t output;
    int linum = __LINE__ + 1;
    LOG_MSG(ERROR, 0, output);

    bool ret = true;

    if (output.sid != 0x183) {
        // Check that the SID was set correctly. Note this assumes
        // that the unit tests assign BOARD_UNIQUE_ID to 0x03
        ret = false;
    } else if (output.data_len != 8) {
        // Check that the proper amount of data was written out
        ret = false;
    } else if (output.data[3] != (0x10 + ((linum >> 8) & 0xF))) {
        // Check that ERROR (0x01) was put in the top nibble, and that
        // bits [11:8] of linum (the line number of where that DEBUG
        // call is above) was put in the bottom nibble
        ret = false;
    } else if (output.data[4] != (linum & 0xFF)) {
        // check that the bottom byte of linum was put in this byte of
        // the data
        ret = false;
    } else if (message_debug_level(&output) != ERROR) {
        // Check the debug level. This is more of a test that the
        // message_debug_level function works, since we already
        // checked that the top nibble of data[3] was 0x01
        ret = false;
    } else if (get_message_type(&output) != MSG_DEBUG_MSG) {
        // Check the message type. Again, we've already checked the
        // SID, so this is more of a check of the get_message_type
        // function
        ret = false;
    }

    return ret;
}

bool test_can_common_functions(void)
{
    bool ret = true;
    if (!test_message_debug_level()) {
        REPORT_FAIL("Error, test_message_debug_level returned false");
        ret = false;
    }
    if (!test_debug_printf()) {
        REPORT_FAIL("Error, test_debug_printf returned false");
        ret = false;
    }
    if (!test_debug_radio_cmd()) {
        REPORT_FAIL("Error, test_debug_radio_cmd returned false");
        ret = false;
    }
    if (!test_debug_macro()) {
        REPORT_FAIL("Error, test_debug_macro returned false");
        ret = false;
    }

    return ret;
}

