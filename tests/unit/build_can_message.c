#include "build_can_message.h"
#include "can_common.h"
#include "message_types.h"
#include <stdio.h>

//if this test is running on hardware, don't actually print anything
#ifdef TARGET_LOCAL
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

    bool ret = true;

    // Test illegal args
    if (build_general_cmd_msg(timestamp, input_data, NULL)) {
        REPORT_FAIL("Message built with null output");
        ret = false;
    }

    // make sure we don't get an error creating the message
    if (!build_general_cmd_msg(timestamp, input_data, &output)) {
        REPORT_FAIL("Error building message");
        ret = false;
    }

    // check that the data length is 4
    if (output.data_len != 4) {
        REPORT_FAIL("Wrong data_len generated");
        ret = false;
    }
    // check that the timestamp was copied across properly
    // only bottom 3 bytes of original should be copied
    if (get_timestamp(&output) != (timestamp & 0xffffff)) {
        REPORT_FAIL("Timestamp copied wrong");
        ret = false;
    }
    // check that the SID is proper. Note that the Makefile defines
    // the unique ID to be 0x3
    if (output.sid != 0x063) {
        REPORT_FAIL("SID compare failed");
        ret = false;
    }

    return ret;
}

static bool test_valve_cmd(void)
{
    uint32_t timestamp = 0x12345678;
    can_msg_t output;

    bool ret = true;

    // test illegal args
    if (build_valve_cmd_msg(timestamp, VALVE_OPEN, MSG_DEBUG_MSG, &output)) {
        REPORT_FAIL("Valve cmd built with invalid message type");
        ret = false;
    }
    if (build_valve_cmd_msg(timestamp, VALVE_OPEN, MSG_INJ_VALVE_CMD, NULL)) {
        REPORT_FAIL("Message built with null output");
        ret = false;
    }

    // test nominal behaviour
    if (!build_valve_cmd_msg(timestamp, VALVE_OPEN, MSG_INJ_VALVE_CMD, &output)) {
        REPORT_FAIL("Error building message");
        ret = false;
    }
    if (output.data_len != 4) {
        REPORT_FAIL("Data length is wrong");
        ret = false;
    }
    if (get_timestamp(&output) != (timestamp & 0xffffff)) {
        REPORT_FAIL("Timestamp copied wrong");
        ret = false;
    }
    if (get_curr_valve_state(&output) != -1) {
        REPORT_FAIL("Got current valve state from a command message");
        ret = false;
    }
    if (get_req_valve_state(&output) != VALVE_OPEN) {
        REPORT_FAIL("Valve data copied incorrectly");
        ret = false;
    }

    return ret;
}

static bool test_valve_stat(void)
{
    uint32_t timestamp = 0x12345678;
    can_msg_t output;

    bool ret = true;

    // test illegal args
    if (build_valve_stat_msg(timestamp, VALVE_OPEN, VALVE_UNK, MSG_DEBUG_MSG, &output)) {
        REPORT_FAIL("Valve cmd built with invalid message type");
        ret = false;
    }
    if (build_valve_stat_msg(timestamp, VALVE_OPEN, VALVE_UNK, MSG_INJ_VALVE_STATUS, NULL)) {
        REPORT_FAIL("Message built with null output");
        ret = false;
    }

    // test nominal behaviour
    if (!build_valve_stat_msg(timestamp, VALVE_OPEN, VALVE_CLOSED, MSG_INJ_VALVE_STATUS, &output)) {
        REPORT_FAIL("Error building message");
        ret = false;
    }
    if (output.data_len != 5) {
        REPORT_FAIL("Data length is wrong");
        ret = false;
    }
    if (get_timestamp(&output) != (timestamp & 0xffffff)) {
        REPORT_FAIL("Timestamp copied wrong");
        ret = false;
    }
    if (get_curr_valve_state(&output) != VALVE_OPEN) {
        REPORT_FAIL("Current valve state copied wrong");
        ret = false;
    }
    if (get_req_valve_state(&output) != VALVE_CLOSED) {
        REPORT_FAIL("Requested valve state copied wrong");
        ret = false;
    }

    return ret;
}

static bool test_board_stat(void)
{
    uint32_t timestamp = 0x12345678;
    uint8_t error_data[2] = { 0x32, 0x10 };
    uint8_t error_data_len = 2;
    can_msg_t output;

    bool ret = true;

    // test illegal args
    if (build_board_stat_msg(timestamp, E_NOMINAL, NULL, error_data_len, &output)) {
        REPORT_FAIL("Built message with null input data");
        ret = false;
    }
    if (build_board_stat_msg(timestamp, E_NOMINAL, error_data, error_data_len, NULL)) {
        REPORT_FAIL("Built message with null output");
        ret = false;
    }

    // test nominal behaviour
    if (!build_board_stat_msg(timestamp, E_BATT_UNDER_VOLTAGE, error_data, error_data_len, &output)) {
        REPORT_FAIL("Error building message");
        ret = false;
    }
    if (get_timestamp(&output) != (timestamp & 0xffffff)) {
        REPORT_FAIL("Timestamp copied wrong");
        ret = false;
    }
    if (output.data_len != 6) {
        REPORT_FAIL("Data length is wrong");
    }

    return ret;
}

static bool test_debug_printf(void)
{
    // put the string "unittest" into a can message
    uint8_t input_data[8] = {
        'u', 'n', 'i', 't',
        't', 'e', 's', 't'
    };

    can_msg_t output;
    bool ret = true;

    // Test illegal args
    if (build_debug_printf(NULL, &output)) {
        REPORT_FAIL("Message built with null data");
        ret = false;
    }

    if (build_debug_printf(input_data, NULL)) {
        REPORT_FAIL("Message built with null output");
        ret = false;
    }

    if (!build_debug_printf(input_data, &output)) {
        REPORT_FAIL("Error building message");
        ret = false;
    }
    // check the SID
    if (output.sid != 0x1E3) {
        REPORT_FAIL("SID compare failed");
        ret = false;
    }
    if (get_timestamp(&output) != 0) {
        REPORT_FAIL("Timestamp copied wrong");
        ret = false;
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
        ret = false;
    }
    return ret;
}

static bool test_sensor_imu(void)
{
    uint16_t input_data[3] = {
        0xa749, 0x6664, 0x1008
    };
    can_msg_t output;
    uint32_t timestamp = 0x12345678;
    bool ret = true;

    // test illegal args
    if (build_imu_data_msg(MSG_GENERAL_CMD, timestamp, input_data, &output)) {
        REPORT_FAIL("IMU message created with invalid message type");
        ret = false;
    }
    if (build_imu_data_msg(MSG_SENSOR_ACC, timestamp, NULL, &output)) {
        REPORT_FAIL("Message built with null input data");
        ret = false;
    }
    if (build_imu_data_msg(MSG_SENSOR_ACC, timestamp, input_data, NULL)) {
        REPORT_FAIL("Message built with null output");
        ret = false;
    }

    // test nominal behaviour
    if (!build_imu_data_msg(MSG_SENSOR_ACC, timestamp, input_data, &output)) {
        REPORT_FAIL("Error building message");
        ret = false;
    }
    if (output.sid != 0x583) {
        REPORT_FAIL("SID compare failed");
        ret = false;
    }
    if (output.data_len != 8) {
        REPORT_FAIL("Data length copied wrong");
        ret = false;
    }
    if (get_timestamp(&output) != (timestamp & 0xffff)) {
        REPORT_FAIL("Timestamp copied wrong");
        ret = false;
    }

    uint16_t output_x, output_y, output_z;
    if (!get_imu_data(&output, &output_x, &output_y, &output_z)) {
        REPORT_FAIL("Failed to retrieve sensor data");
        ret = false;
    }
    if (output_z != 0x1008) {
        REPORT_FAIL("Z data is incorrect");
        ret = false;
    }
    if (output_y != 0x6664) {
        REPORT_FAIL("Y data is incorrect");
        ret = false;
    }
    if (output_x != 0xa749) {
        REPORT_FAIL("X data is incorrect");
        ret = false;
    }
    return ret;
}

static bool test_sensor_analog(void)
{
    uint16_t data = 0x380c;
    uint32_t timestamp = 0x12345678;
    can_msg_t output;
    bool ret = true;

    // test illegal args
    if (build_analog_data_msg(timestamp, SENSOR_BARO, data, NULL)) {
        REPORT_FAIL("Message built with null output");
        ret = false;
    }

    // test nominal behaviour
    if (!build_analog_data_msg(timestamp, SENSOR_BARO, data, &output)) {
        REPORT_FAIL("Error building message");
        ret = false;
    }
    if (output.sid != 0x6a3) {
        REPORT_FAIL("SID compare failed");
        ret = false;
    }
    if (output.data_len != 5) {
        REPORT_FAIL("Data length copied wrong");
        ret = false;
    }
    if (get_timestamp(&output) != (timestamp & 0xffff)) {
        REPORT_FAIL("Timestamp copied wrong");
        ret = false;
    }

    uint16_t output_data;
    enum SENSOR_ID output_sensor_id;
    if (!get_analog_data(&output, &output_sensor_id, &output_data)) {
        REPORT_FAIL("Failed to retrieve sensor data");
        ret = false;
    }
    if (output_data != data) {
        REPORT_FAIL("Analog data is incorrect");
        ret = false;
    }
    if (output_sensor_id != SENSOR_BARO) {
        REPORT_FAIL("Sensor id is incorrect");
        ret = false;
    }
    return ret;
}

bool test_build_can_message(void)
{
    bool ret = true;
    if (!test_general_command()) {
        REPORT_FAIL("test_general_command returned false");
        ret = false;
    }
    if (!test_valve_cmd()) {
        REPORT_FAIL("test_valve_cmd returned false");
        ret = false;
    }
    if (!test_valve_stat()) {
        REPORT_FAIL("test_valve_stat returned false");
        ret = false;
    }
    if (!test_board_stat()) {
        REPORT_FAIL("test_board_stat returned false");
        ret = false;
    }
    if (!test_debug_printf()) {
        REPORT_FAIL("test_debug_printf returned false");
        ret = false;
    }
    if (!test_sensor_imu()) {
        REPORT_FAIL("test_sensor_acc returned false");
        ret = false;
    }
    if (!test_sensor_analog()) {
        REPORT_FAIL("test_sensor_analog returned false");
        ret = false;
    }

    return ret;
}
