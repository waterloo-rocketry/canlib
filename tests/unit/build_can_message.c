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

static bool test_general_command(void)
{
    // make sure that a MSG_GENERAL_CMD message works properly
    uint32_t timestamp = 0xcafebabe;
    uint8_t input_data = 0x74;
    can_msg_t output;

    // make sure we don't get an error creating the message
    build_general_cmd_msg(timestamp, input_data, &output);

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
    build_debug_printf(input_data, &output);
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
    uint16_t input_data[3] = {
        0xa749, 0x6664, 0x1008
    };
    can_msg_t output;
    uint32_t timestamp = 0x12345678;
    build_imu_data_msg(MSG_SENSOR_ACC, timestamp, input_data, &output);
    
    if (output.sid != 0x583) {
        REPORT_FAIL("SID compare failed");
        printf("sid: %x\n", output.sid);
        return false;
    }
    if (output.data[0] != 0x56 ||
        output.data[1] != 0x78) {
        REPORT_FAIL("Timestamp copied wrong");
        return false;
    }
    if (output.data[2] != 0xa7 ||
        output.data[3] != 0x49 ||
        output.data[4] != 0x66 ||
        output.data[5] != 0x64 ||
        output.data[6] != 0x10 ||
        output.data[7] != 0x08) {
        REPORT_FAIL("Sensor Data copied wrong");
        return false;
    }
    return true;
}


bool test_build_can_message(void)
{
    if (!test_general_command()) {
        printf(__FILE__ ": Error, test_general_command returned false\n");
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
