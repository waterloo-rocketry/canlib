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

    // check that the command itself was copied properly
    if (get_general_cmd_type(&output) != input_data) {
        REPORT_FAIL("Command type copied wrong");
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

static bool test_reset_command(void)
{
    // make sure that a MSG_RESET_CMD message works properly
    uint32_t timestamp = 0xcafebabe;
    uint8_t input_board_id = 0x1F;
    can_msg_t output;

    bool ret = true;

    // Test illegal args
    if (build_reset_msg(timestamp, input_board_id, NULL)) {
        REPORT_FAIL("Message built with null output");
        ret = false;
    }

    // make sure we don't get an error creating the message
    if (!build_reset_msg(timestamp, input_board_id, &output)) {
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

    // check that the command itself was copied properly
    if (get_reset_board_id(&output) != input_board_id) {
        REPORT_FAIL("Command type copied wrong");
        ret = false;
    }

    // check that the SID is proper. Note that the Makefile defines
    // the unique ID to be 0x3
    if (output.sid != 0x163) {
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

static bool test_arm_cmd(void){
    uint32_t timestamp = 0x12345678;
    can_msg_t output;

    bool ret = true;

    uint8_t alt_number = 1;

    // test illegal args
    if (build_arm_cmd_msg(timestamp, alt_number, ARMED, NULL)) {
        REPORT_FAIL("Message built with null output");
        ret = false;
    }

    // test nominal behaviour
    if (!build_arm_cmd_msg(timestamp, alt_number, ARMED, &output)) {
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

    uint8_t copied_alt_num;
    enum ARM_STATE copied_arming_state;
    if (get_curr_arm_state(&output, &copied_alt_num, &copied_arming_state)) {
        REPORT_FAIL("Got current arming state from a command message");
        ret = false;
    }

    if (!get_req_arm_state(&output, &copied_alt_num, &copied_arming_state)){
        REPORT_FAIL("Failed to get requested arm state");
        ret = false;
    }

    if (alt_number != copied_alt_num) {
        REPORT_FAIL("Altimeter number copied incorrectly");
        ret = false;
    }
    if (copied_arming_state != ARMED){
        REPORT_FAIL("Altimeter requested state copied incorrectly");
        ret = false;
    }

    return ret;
}

static bool test_arm_status(void){
    uint32_t timestamp = 0x12345678;
    can_msg_t output;

    bool ret = true;

    uint8_t alt_number = 1;
    uint16_t v_drogue = 0xcafe, v_main = 0xdada;

    // test illegal args
    if (build_arm_stat_msg(timestamp, alt_number, ARMED,
                           v_drogue, v_main, NULL)) {
        REPORT_FAIL("Message built with null output");
        ret = false;
    }

    // test nominal behaviour
    if (!build_arm_stat_msg(timestamp, alt_number, ARMED,
                            v_drogue, v_main, &output)) {
        REPORT_FAIL("Error building message");
        ret = false;
    }
    if (output.data_len != 8) {
        REPORT_FAIL("Data length is wrong");
        ret = false;
    }
    if (get_timestamp(&output) != (timestamp & 0xffffff)) {
        REPORT_FAIL("Timestamp copied wrong");
        ret = false;
    }

    uint8_t copied_alt_num;
    enum ARM_STATE copied_arming_state;
    if (get_req_arm_state(&output, &copied_alt_num, &copied_arming_state)) {
        REPORT_FAIL("Got requested arming state from a status message");
        ret = false;
    }

    if (!get_curr_arm_state(&output, &copied_alt_num, &copied_arming_state)){
        REPORT_FAIL("Failed to get current arm state");
        ret = false;
    }

    if (alt_number != copied_alt_num) {
        REPORT_FAIL("Altimeter number copied incorrectly");
        ret = false;
    }
    if (copied_arming_state != ARMED){
        REPORT_FAIL("Altimeter requested state copied incorrectly");
        ret = false;
    }

    uint16_t copied_v_drogue, copied_v_main;
    get_pyro_voltage_data(&output, &copied_v_drogue, &copied_v_main);
    if (copied_v_drogue != v_drogue){
        REPORT_FAIL("Drogue voltage copied incorrectly");
        ret = false;
    }
    if (copied_v_main != v_main){
        REPORT_FAIL("Main voltage copied incorrectly");
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

static bool test_sensor_altitude(void)
{
    int32_t altitude = 30000;
    uint32_t timestamp = 0x12345678;
    can_msg_t output;
    bool ret = true;

    // test illegal args
    if (build_altitude_data_msg(timestamp, altitude, NULL)) {
        REPORT_FAIL("Message built with null output");
        ret = false;
    }

    // test nominal behaviour
    if (!build_altitude_data_msg(timestamp, altitude, &output)) {
        REPORT_FAIL("Error building message");
        ret = false;
    }
    if (output.sid != 0x563) {
        REPORT_FAIL("SID compare failed");
        ret = false;
    }
    if (output.data_len != 7) {
        REPORT_FAIL("Data length copied wrong");
        ret = false;
    }
    if (get_timestamp(&output) != (timestamp & 0xffffff)) {
        REPORT_FAIL("Timestamp copied wrong");
        ret = false;
    }

    int32_t output_altitude;
    if (!get_altitude_data(&output, &output_altitude)) {
        REPORT_FAIL("Failed to retrieve sensor data");
        ret = false;
    }
    if (output_altitude != altitude) {
        REPORT_FAIL("Altitude data is incorrect");
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

bool test_gps(void)
{
    // global timestamp
    uint32_t timestamp = 0x472032;

    // test utc time
    uint8_t utc_hours = 21;
    uint8_t utc_mins = 56;
    uint8_t utc_secs = 32;
    uint8_t utc_dsecs = 9;

    // test lat/lon
    uint8_t degrees = 145;
    uint8_t minutes = 45;
    uint16_t dminutes = 2300;
    uint8_t direction_ns = 'S';
    uint8_t direction_ew = 'E';

    // test altitude
    uint16_t altitude = 45000;
    uint8_t daltitude = 9;
    uint8_t units = 'F';

    // test general info
    uint8_t num_sat = 18;
    uint8_t quality = 5;

    can_msg_t output;
    bool ret = true;

    // illegal args
    if (build_gps_time_msg(0, 0, 0, 0, 0, NULL)) {
        REPORT_FAIL("Built message with null input");
        ret = false;
    }
    if (build_gps_lat_msg(0, 0, 0, 0, 0, NULL)) {
        REPORT_FAIL("Built message with null input");
        ret = false;
    }
    if (build_gps_lon_msg(0, 0, 0, 0, 0, NULL)) {
        REPORT_FAIL("Built message with null input");
        ret = false;
    }
    if (build_gps_alt_msg(0, 0, 0, 0, NULL)) {
        REPORT_FAIL("Built message with null input");
        ret = false;
    }
    if (build_gps_info_msg(0, 0, 0, NULL)) {
        REPORT_FAIL("Built message with null input");
        ret = false;
    }

    // expected behaviour - time
    if (!build_gps_time_msg(timestamp, utc_hours, utc_mins, utc_secs, utc_dsecs, &output)) {
        REPORT_FAIL("Error building CAN message");
        ret = false;
    }

    uint8_t utc_hours_, utc_mins_, utc_secs_, utc_dsecs_;
    uint32_t timestamp_ = get_timestamp(&output);
    if (!get_gps_time(&output, &utc_hours_, &utc_mins_, &utc_secs_, &utc_dsecs_)) {
        REPORT_FAIL("Error getting GPS time fields");
        ret = false;
    }
    if ((timestamp != timestamp_) || (utc_hours != utc_hours_) || (utc_mins != utc_mins_)
        || (utc_secs != utc_secs_) || (utc_dsecs != utc_dsecs_)) {
        REPORT_FAIL("GPS time fields do not match");
        ret = false;
    }

    // expected behaviour - latitude
    if (!build_gps_lat_msg(timestamp, degrees, minutes, dminutes, direction_ns, &output)) {
        REPORT_FAIL("Error building CAN message");
        ret = false;
    }

    uint8_t degrees_, minutes_, direction_;
    uint16_t dminutes_;
    timestamp_ = get_timestamp(&output);
    if (!get_gps_lat(&output, &degrees_, &minutes_, &dminutes_, &direction_)) {
        REPORT_FAIL("Error getting latitude data");
        ret = false;
    }
    if ((timestamp != timestamp_) || (degrees != degrees_) || (minutes != minutes_)
        || (dminutes != dminutes_) || (direction_ns != direction_)) {
        REPORT_FAIL("GPS latitude fields do not match");
        ret = false;
    }

    // expected behaviour - longitude
    if (!build_gps_lon_msg(timestamp, degrees, minutes, dminutes, direction_ew, &output)) {
        REPORT_FAIL("Error building CAN message");
        ret = false;
    }
    timestamp_ = get_timestamp(&output);
    if (!get_gps_lon(&output, &degrees_, &minutes_, &dminutes_, &direction_)) {
        REPORT_FAIL("Error getting longitude data");
        ret = false;
    }
    if ((timestamp != timestamp_) || (degrees != degrees_) || (minutes != minutes_)
        || (dminutes != dminutes_) || (direction_ew != direction_)) {
        REPORT_FAIL("GPS longitude fields do not match");
        ret = false;
    }

    // expected behaviour - altitude
    if (!build_gps_alt_msg(timestamp, altitude, daltitude, units, &output)) {
        REPORT_FAIL("Error building CAN message");
        ret = false;
    }

    uint16_t altitude_;
    uint8_t daltitude_, units_;
    timestamp_ = get_timestamp(&output);
    if (!get_gps_alt(&output, &altitude_, &daltitude_, &units_)) {
        REPORT_FAIL("Error getting latitude data");
        ret = false;
    }
    if ((timestamp != timestamp_) || (altitude != altitude_) || (daltitude != daltitude_)
        || (units != units_)) {
        REPORT_FAIL("GPS latitude fields do not match");
        ret = false;
    }

    // expected behaviour - info
    if (!build_gps_info_msg(timestamp, num_sat, quality, &output)) {
        REPORT_FAIL("Error building CAN message");
        ret = false;
    }
    uint8_t num_sat_, quality_;
    timestamp_ = get_timestamp(&output);
    if (!get_gps_info(&output, &num_sat_, &quality_)) {
        REPORT_FAIL("Error getting GPS info data");
        ret = false;
    }
    if ((timestamp != timestamp_) || (num_sat != num_sat_) || (quality != quality_)) {
        REPORT_FAIL("GPS info fields do not match");
        ret = false;
    }

    return ret;
}

bool test_build_fill_message(void)
{
    uint32_t timestamp = 0x12345678;
    can_msg_t output;
    uint8_t fill_lvl = 8;
    uint8_t dir = FILLING;

    bool ret = true;

    // test illegal args
    if(build_fill_msg(timestamp, fill_lvl, dir, NULL)){
        REPORT_FAIL("Built with null output");
        ret = false;
    }

    // test nominal behaviour
    if(!build_fill_msg(timestamp, fill_lvl, dir, &output)){
        REPORT_FAIL("Error building fill message");
        ret = false;
    }

    if(output.data_len != 5){
        REPORT_FAIL("Wrong data length generated");
        ret = false;
    }

    uint8_t rx_lvl;
    uint8_t rx_dir;
    if(!get_fill_info(&output, &rx_lvl, &rx_dir)){
        REPORT_FAIL("Error getting fill info");
        ret = false;
    }

    if(rx_lvl != fill_lvl || rx_dir != dir){
        REPORT_FAIL("Fill info fields don't match");
        ret = false;
    }

    if (get_timestamp(&output) != (timestamp & 0xffffff)) {
        REPORT_FAIL("Timestamp copied wrong");
        ret = false;
    }

    return ret;
}

bool test_build_radi_info_msg (void)
{
    uint32_t timestamp = 0x123456;
    can_msg_t output;
    uint8_t sensor_identifier = 3;
    uint16_t adc_value = 2021;

    bool ret = true;

    //test illegal args
    if (build_radi_info_msg(timestamp, sensor_identifier, adc_value, NULL))
    {
        REPORT_FAIL("Built with null output");
        ret = false;
    }

    //test nominal behaviour
    if (!build_radi_info_msg(timestamp, sensor_identifier, adc_value, &output))
    {
        REPORT_FAIL("Error building radiation board message");
        ret = false;
    }

    if (output.data_len != 6)
    {
        REPORT_FAIL ("Wrong data length generated");
        ret = false;
    }

    uint8_t test_sensor_identifier;
    uint16_t test_adc_value;

    if (!get_radi_info(&output, &test_sensor_identifier, &test_adc_value))
    {
        REPORT_FAIL("Error getting radiation board message");
        ret = false;
    }

    if (test_sensor_identifier != sensor_identifier || test_adc_value != adc_value)
    {
        REPORT_FAIL ("Radiation board fields dont match");
        ret = false;
    }

    if (get_timestamp(&output) != (timestamp & 0xffffff)) {
        REPORT_FAIL("Timestamp copied wrong");
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
    if (!test_reset_command()) {
        REPORT_FAIL("test_reset_command returned false");
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
    if (!test_arm_cmd()){
        REPORT_FAIL("test_arm_cmd returned false");
        ret = false;
    }
    if (!test_arm_status()){
        REPORT_FAIL("test_arm_status returned false");
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
    if(!test_sensor_altitude()) {
        REPORT_FAIL("test_sensor_altitude returned false");
        ret = false;
    }
    if (!test_sensor_imu()) {
        REPORT_FAIL("test_sensor_imu returned false");
        ret = false;
    }
    if (!test_sensor_analog()) {
        REPORT_FAIL("test_sensor_analog returned false");
        ret = false;
    }
    if (!test_gps()) {
        REPORT_FAIL("test_gps returned false");
        ret = false;
    }
    if (!test_build_fill_message()) {
        REPORT_FAIL("test_build_fill_msg returned false");
    }
    if (!test_build_radi_info_msg()){
        REPORT_FAIL("test_build_radi_info_msg returned false");
        ret = false;
    }


    return ret;
}
