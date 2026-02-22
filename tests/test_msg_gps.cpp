#include <cstdint>

#include "rockettest.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_gps.h"
#include "message_types.h"

class gps_timestamp_message_test : rockettest_test {
public:
    gps_timestamp_message_test() : rockettest_test("gps_timestamp_message_test") {}

    bool run_test() override {
        bool test_passed = true;

        can_msg_t msg;

        can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>(); 
        std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>(); //byte 0-1
        std::uint8_t utc_hours_before = rockettest_rand<std::uint8_t, 0xff>(); //byte 2
        std::uint8_t utc_mins_before = rockettest_rand<std::uint8_t, 0xff>(); //byte 3
        std::uint8_t utc_secs_before = rockettest_rand<std::uint8_t, 0xff>(); //byte 4
        std::uint8_t utc_dsecs_before = rockettest_rand<std::uint8_t, 0xff>(); //byte 5

        build_gps_time_msg(prio_before, timestamp_before, utc_hours_before, utc_mins_before, utc_secs_before, utc_dsecs_before, &msg);

        std::uint16_t timestamp_extracted;
        std::uint8_t utc_hours_extracted;
        std::uint8_t utc_mins_extracted;
        std::uint8_t utc_secs_extracted;
        std::uint8_t utc_dsecs_extracted;

        timestamp_extracted = (static_cast<std::uint16_t>(msg.data[0]) << 8) | msg.data[1];
        utc_hours_extracted = msg.data[2];
        utc_mins_extracted = msg.data[3];
        utc_secs_extracted = msg.data[4];
        utc_dsecs_extracted = msg.data[5];

        rockettest_check_expr_true(msg.data_len == 6);
        rockettest_check_expr_true(timestamp_extracted == timestamp_before);
        rockettest_check_expr_true(utc_hours_extracted == utc_hours_before);
        rockettest_check_expr_true(utc_mins_extracted == utc_mins_before);
        rockettest_check_expr_true(utc_secs_extracted == utc_secs_before);
        rockettest_check_expr_true(utc_dsecs_extracted == utc_dsecs_before);
        rockettest_check_expr_true(timestamp_extracted == timestamp_before);

        can_msg_type_t type_after;
        std::uint16_t timestamp_after;
        uint8_t utc_hours_after;
        uint8_t utc_mins_after;
        uint8_t utc_secs_after;
        uint8_t utc_dsecs_after;

        type_after = get_message_type(&msg);
        timestamp_after = get_timestamp(&msg);
        get_gps_time(&msg, &utc_hours_after, &utc_mins_after, &utc_secs_after, &utc_dsecs_after);

        rockettest_check_expr_true(type_after == MSG_GPS_TIMESTAMP);
        rockettest_check_expr_true(timestamp_after == timestamp_before);
        rockettest_check_expr_true(utc_hours_after == utc_hours_before);
        rockettest_check_expr_true(utc_mins_after == utc_mins_before);
        rockettest_check_expr_true(utc_secs_after == utc_secs_before);
        rockettest_check_expr_true(utc_dsecs_after == utc_dsecs_before);

        return test_passed;
    }
};

gps_timestamp_message_test gps_timestamp_message_test_inst;

class gps_latitude_message_test : rockettest_test {
public:
    gps_latitude_message_test() : rockettest_test("gps_latitude_message_test") {}

    bool run_test() override {
        bool test_passed = true;

        can_msg_t msg;

        can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
        std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
        std::uint8_t degrees_before = rockettest_rand<std::uint8_t, 0xff>();
        std::uint8_t minutes_before = rockettest_rand<std::uint8_t, 0xff>();
        std::uint16_t dminutes_before = rockettest_rand<std::uint16_t>();
        std::uint8_t direction_before = rockettest_rand<std::uint8_t, 0xff>();

        build_gps_lat_msg(prio_before, timestamp_before, degrees_before, minutes_before, dminutes_before, direction_before, &msg);

        std::uint16_t timestamp_extracted;
        std::uint8_t degrees_extracted;
        std::uint8_t minutes_extracted;
        std::uint16_t dminutes_extracted;
        std::uint8_t direction_extracted;

        timestamp_extracted = (static_cast<std::uint16_t>(msg.data[0]) << 8) | msg.data[1];
        degrees_extracted = msg.data[2];
        minutes_extracted = msg.data[3];
        dminutes_extracted = (static_cast<std::uint16_t>(msg.data[4]) << 8) | msg.data[5];
        direction_extracted = msg.data[6];

        rockettest_check_expr_true(msg.data_len == 7);
        rockettest_check_expr_true(timestamp_extracted == timestamp_before);
        rockettest_check_expr_true(degrees_extracted == degrees_before);
        rockettest_check_expr_true(minutes_extracted == minutes_before);
        rockettest_check_expr_true(dminutes_extracted == dminutes_before);
        rockettest_check_expr_true(direction_extracted == direction_before);

        can_msg_type_t type_after;
        std::uint16_t timestamp_after;
        uint8_t degrees_after;
        uint8_t minutes_after;
        uint16_t dminutes_after;
        uint8_t direction_after;

        type_after = get_message_type(&msg);
        timestamp_after = get_timestamp(&msg);
        get_gps_lat(&msg, &degrees_after, &minutes_after, &dminutes_after, &direction_after);

        rockettest_check_expr_true(type_after == MSG_GPS_LATITUDE);
        rockettest_check_expr_true(timestamp_after == timestamp_before);
        rockettest_check_expr_true(degrees_after == degrees_before);
        rockettest_check_expr_true(minutes_after == minutes_before);
        rockettest_check_expr_true(dminutes_after == dminutes_before);
        rockettest_check_expr_true(direction_after == direction_before);
        
        return test_passed;
    }
};

gps_latitude_message_test gps_latitude_message_test_inst;

class gps_longitude_message_test : rockettest_test {
public:
    gps_longitude_message_test() : rockettest_test("gps_longitude_message_test") {}

    bool run_test() override {
        bool test_passed = true;

        can_msg_t msg;

        can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
        std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
        std::uint8_t degrees_before = rockettest_rand<std::uint8_t, 0xff>();
        std::uint8_t minutes_before = rockettest_rand<std::uint8_t, 0xff>();
        std::uint16_t dminutes_before = rockettest_rand<std::uint16_t>();
        std::uint8_t direction_before = rockettest_rand<std::uint8_t, 0xff>();

        build_gps_lon_msg(prio_before, timestamp_before, degrees_before, minutes_before, dminutes_before, direction_before, &msg);

        std::uint16_t timestamp_extracted;
        std::uint8_t degrees_extracted;
        std::uint8_t minutes_extracted;
        std::uint16_t dminutes_extracted;
        std::uint8_t direction_extracted;

        timestamp_extracted = (static_cast<std::uint16_t>(msg.data[0]) << 8) | msg.data[1];
        degrees_extracted = msg.data[2];
        minutes_extracted = msg.data[3];
        dminutes_extracted = (static_cast<std::uint16_t>(msg.data[4]) << 8) | msg.data[5];
        direction_extracted = msg.data[6];

        rockettest_check_expr_true(msg.data_len == 7);
        rockettest_check_expr_true(timestamp_extracted == timestamp_before);
        rockettest_check_expr_true(degrees_extracted == degrees_before);
        rockettest_check_expr_true(minutes_extracted == minutes_before);
        rockettest_check_expr_true(dminutes_extracted == dminutes_before);
        rockettest_check_expr_true(direction_extracted == direction_before);

        can_msg_type_t type_after;
        std::uint16_t timestamp_after;
        uint8_t degrees_after;
        uint8_t minutes_after;
        uint16_t dminutes_after;
        uint8_t direction_after;

        type_after = get_message_type(&msg);
        timestamp_after = get_timestamp(&msg);
        get_gps_lon(&msg, &degrees_after, &minutes_after, &dminutes_after, &direction_after);
        
        rockettest_check_expr_true(type_after == MSG_GPS_LONGITUDE);
        rockettest_check_expr_true(timestamp_after == timestamp_before);
        rockettest_check_expr_true(degrees_after == degrees_before);
        rockettest_check_expr_true(minutes_after == minutes_before);
        rockettest_check_expr_true(dminutes_after == dminutes_before);
        rockettest_check_expr_true(direction_after == direction_before);

        return test_passed;
    }
};

gps_longitude_message_test gps_longitude_message_test_inst;

class gps_altitude_message_test : rockettest_test {
public:
    gps_altitude_message_test() : rockettest_test("gps_altitude_message_test") {}

    bool run_test() override {
        bool test_passed = true;

        can_msg_t msg;

        can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
        std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
        std::uint16_t altitude_before = rockettest_rand<std::uint16_t>();
        std::uint8_t daltitude_before = rockettest_rand<std::uint8_t>();
        std::uint8_t units_before = rockettest_rand<std::uint8_t>();

        build_gps_alt_msg(prio_before, timestamp_before, altitude_before, daltitude_before, units_before, &msg);

        std::uint16_t timestamp_extracted;
        std::uint16_t altitude_extracted;
        std::uint8_t daltitude_extracted;
        std::uint8_t units_extracted;

        timestamp_extracted = (static_cast<std::uint16_t>(msg.data[0]) << 8) | msg.data[1];
        altitude_extracted = (static_cast<std::uint16_t>(msg.data[2]) << 8) | msg.data[3];
        daltitude_extracted = msg.data[4];
        units_extracted = msg.data[5];

        rockettest_check_expr_true(msg.data_len == 6);
        rockettest_check_expr_true(timestamp_extracted == timestamp_before);
        rockettest_check_expr_true(altitude_extracted == altitude_before);
        rockettest_check_expr_true(daltitude_extracted == daltitude_before);
        rockettest_check_expr_true(units_extracted == units_before);

        can_msg_type_t type_after;
        std::uint16_t timestamp_after;
        uint16_t altitude_after;
        uint8_t daltitude_after;
        uint8_t units_after;

        type_after = get_message_type(&msg);
        timestamp_after = get_timestamp(&msg);
        get_gps_alt(&msg, &altitude_after, &daltitude_after, &units_after);

        rockettest_check_expr_true(type_after == MSG_GPS_ALTITUDE);
        rockettest_check_expr_true(timestamp_after == timestamp_before);
        rockettest_check_expr_true(altitude_after == altitude_before);
        rockettest_check_expr_true(daltitude_after == daltitude_before);
        rockettest_check_expr_true(units_after == units_before);
        
        return test_passed;
    }
};

gps_altitude_message_test gps_altitude_message_test_inst;