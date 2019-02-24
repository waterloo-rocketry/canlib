#include "build_can_message.h"
#include "can_common.h"
#include "message_types.h"
#include <stdio.h>

//if this test is running on hardware, don't actually print anything
#ifndef NO_PRINTF
#define REPORT_FAIL(x) printf("%s: Fail: %s\n", __FUNCTION__, x)
#else
#define REPORT_FAIL(x)
#endif

static bool test_illegal_arguments(void)
{
    can_msg_t output;
    uint8_t input_data[8];
    // make sure illegal message type generates error
    if (build_can_message(0, 0, input_data, &output)) {
        REPORT_FAIL("No error when passing 0 as message type");
        return false;
    }
    // make sure that passing null pointer as input data generates
    // error
    else if (build_can_message(MSG_GENERAL_CMD, 0, NULL, &output)) {
        REPORT_FAIL("No error when passing NULL as input data pointer");
        return false;
    }
    // make sure that passing null pointer as output data generates an
    // error
    else if (build_can_message(MSG_GENERAL_CMD, 0, input_data, NULL)) {
        REPORT_FAIL("No error when passing NULL as output pointer");
        return false;
    }

    // if those three cases all passed, we can safely return true
    return true;
}

static bool test_general_command(void)
{
    // make sure that a MSG_GENERAL_CMD message works properly
    uint32_t timestamp = 0xcafebabe;
    uint8_t input_data[1] = { 0x74 };
    can_msg_t output;
    // make sure we don't get an error creating the message
    if (!build_can_message(MSG_GENERAL_CMD, timestamp, input_data, &output)) {
        REPORT_FAIL("Error building message");
        return false;
    }
    // check that the data length is 4
    if (output.data_len != 4) {
        REPORT_FAIL("Wrong data_len generated");
        return false;
    }
    // check that the timestamp was copied across properly
    if (output.data[0] != 0xfe ||
        output.data[1] != 0xba ||
        output.data[2] != 0xbe) {
        REPORT_FAIL("Timestamp copied wrong");
        return false;
    }
    // check that the SID is proper. Note that the Makefile defines
    // the unique ID to be 0x3
    if (output.sid != 0x063) {
        REPORT_FAIL("SID compare failed");
        return false;
    }
    return true;
}

static bool test_debug_printf(void)
{
    // put the string "unittest" into a can message
    uint8_t input_data[8] = {
        'u', 'n', 'i', 't',
        't', 'e', 's', 't'
    };
    can_msg_t output;
    if (!build_can_message(MSG_DEBUG_PRINTF, 0, input_data, &output)) {
        REPORT_FAIL("Error building message");
        return false;
    }
    // check the SID
    if (output.sid != 0x1E3) {
        REPORT_FAIL("SID compare failed");
        return false;
    }
    // check the outputted data
    if (output.data[0] != 'u' ||
        output.data[1] != 'n' ||
        output.data[2] != 'i' ||
        output.data[3] != 't' ||
        output.data[4] != 't' ||
        output.data[5] != 'e' ||
        output.data[6] != 's' ||
        output.data[7] != 't') {
        REPORT_FAIL("printf data not copied over properly");
        return false;
    }
    return true;
}

static bool test_sensor_acc(void)
{
    uint8_t input_data[6] = {
        177, 107, 0, 189, 186, 190
    };
    can_msg_t output;
    uint32_t timestamp = 0x12345678;
    if (!build_can_message(MSG_SENSOR_ACC, timestamp, input_data, &output)) {
        REPORT_FAIL("Error building message");
        return false;
    }
    if (output.sid != 0x583) {
        REPORT_FAIL("SID compare failed");
        return false;
    }
    if (output.data[0] != 0x56 ||
        output.data[1] != 0x78) {
        REPORT_FAIL("Timestamp copied wrong");
        return false;
    }
    if (output.data[2] != 177 ||
        output.data[3] != 107 ||
        output.data[4] != 0   ||
        output.data[5] != 189 ||
        output.data[6] != 186 ||
        output.data[7] != 190) {
        REPORT_FAIL("Sensor Data copied wrong");
        return false;
    }
    return true;
}


bool test_build_can_message(void)
{
    if (!test_illegal_arguments()) {
        REPORT_FAIL("test_illegal_arguments returned false");
        return false;
    } else if (!test_general_command()) {
        REPORT_FAIL("test_general_command returned false");
        return false;
    } else if (!test_debug_printf()) {
        REPORT_FAIL("test_debug_printf returned false");
        return false;
    } else if (!test_sensor_acc()) {
        REPORT_FAIL("test_sensor_acc returned false");
        return false;
    }

    return true;
}
