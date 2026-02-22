#include <cstdint>

#include "rockettest.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_sensor.h"
#include "message_types.h"

class analog_sensor_16bit_message_test : rockettest_test {
public:
	analog_sensor_16bit_message_test() : rockettest_test("analog_sensor_16bit_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		can_analog_sensor_id_t sensor_id_before = rockettest_rand<can_analog_sensor_id_t, 0xff>();
		std::uint16_t sensor_data_before = rockettest_rand<std::uint16_t>();

		build_analog_data_16bit_msg(
			prio_before, timestamp_before, sensor_id_before, sensor_data_before, &msg);

		can_analog_sensor_id_t sensor_id_extracted;
		std::uint16_t sensor_data_extracted;

		sensor_id_extracted = static_cast<can_analog_sensor_id_t>(msg.sid & 0xff);
		sensor_data_extracted = (msg.data[2] << 8) | msg.data[3];

		rockettest_check_expr_true(msg.data_len == 4);
		rockettest_check_expr_true(sensor_id_extracted == sensor_id_before);
		rockettest_check_expr_true(sensor_data_extracted == sensor_data_before);

		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_analog_sensor_id_t sensor_id_after;
		std::uint16_t sensor_data_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_analog_data_16bit(&msg, &sensor_id_after, &sensor_data_after);

		rockettest_check_expr_true(msg_is_sensor_data_after == true);
		rockettest_check_expr_true(type_after == MSG_SENSOR_ANALOG16);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(sensor_id_after == sensor_id_before);
		rockettest_check_expr_true(sensor_data_after == sensor_data_before);

		return test_passed;
	}
};

analog_sensor_16bit_message_test analog_sensor_16bit_message_test_inst;

class analog_sensor_32bit_message_test : rockettest_test {
public:
	analog_sensor_32bit_message_test() : rockettest_test("analog_sensor_32bit_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		can_analog_sensor_id_t sensor_id_before = rockettest_rand<can_analog_sensor_id_t, 0xff>();
		std::uint32_t sensor_data_before =
			(static_cast<std::uint32_t>(rockettest_rand<uint16_t>()) << 16) |
			rockettest_rand<uint16_t>();

		build_analog_data_32bit_msg(
			prio_before, timestamp_before, sensor_id_before, sensor_data_before, &msg);

		can_analog_sensor_id_t sensor_id_extracted;
		std::uint32_t sensor_data_extracted;

		sensor_id_extracted = static_cast<can_analog_sensor_id_t>(msg.sid & 0xff);
		sensor_data_extracted = (static_cast<std::uint32_t>(msg.data[2]) << 24) |
								(static_cast<std::uint32_t>(msg.data[3]) << 16) |
								(static_cast<std::uint32_t>(msg.data[4]) << 8) |
								(static_cast<std::uint32_t>(msg.data[5]) << 0);

		rockettest_check_expr_true(msg.data_len == 6);
		rockettest_check_expr_true(sensor_id_extracted == sensor_id_before);
		rockettest_check_expr_true(sensor_data_extracted == sensor_data_before);

		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_analog_sensor_id_t sensor_id_after;
		std::uint32_t sensor_data_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_analog_data_32bit(&msg, &sensor_id_after, &sensor_data_after);

		rockettest_check_expr_true(msg_is_sensor_data_after == true);
		rockettest_check_expr_true(type_after == MSG_SENSOR_ANALOG32);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(sensor_id_after == sensor_id_before);
		rockettest_check_expr_true(sensor_data_after == sensor_data_before);

		return test_passed;
	}
};

analog_sensor_32bit_message_test analog_sensor_32bit_message_test_inst;

class analog_sensor_dem_16bit_message_test : rockettest_test {
public:
	analog_sensor_dem_16bit_message_test()
		: rockettest_test("analog_sensor_dem_16bit_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		can_dem_sensor_id_t sensor_id_before = rockettest_rand<can_dem_sensor_id_t, 0xff>();
		std::uint16_t sensor_data_x_before = rockettest_rand<std::uint16_t>();
		std::uint16_t sensor_data_y_before = rockettest_rand<std::uint16_t>();
		std::uint16_t sensor_data_z_before = rockettest_rand<std::uint16_t>();

		build_dem_analog_data_16bit_msg(prio_before,
										timestamp_before,
										sensor_id_before,
										sensor_data_x_before,
										sensor_data_y_before,
										sensor_data_z_before,
										&msg);

		can_dem_sensor_id_t sensor_id_extracted;
		std::uint16_t sensor_data_x_extracted;
		std::uint16_t sensor_data_y_extracted;
		std::uint16_t sensor_data_z_extracted;

		sensor_id_extracted = static_cast<can_dem_sensor_id_t>(msg.sid & 0xff);
		sensor_data_x_extracted = (msg.data[2] << 8) | msg.data[3];
		sensor_data_y_extracted = (msg.data[4] << 8) | msg.data[5];
		sensor_data_z_extracted = (msg.data[6] << 8) | msg.data[7];

		rockettest_check_expr_true(msg.data_len == 8);
		rockettest_check_expr_true(sensor_id_extracted == sensor_id_before);
		rockettest_check_expr_true(sensor_data_x_extracted == sensor_data_x_before);
		rockettest_check_expr_true(sensor_data_y_extracted == sensor_data_y_before);
		rockettest_check_expr_true(sensor_data_z_extracted == sensor_data_z_before);

		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_dem_sensor_id_t sensor_id_after;
		std::uint16_t sensor_data_x_after;
		std::uint16_t sensor_data_y_after;
		std::uint16_t sensor_data_z_after;
		get_dem_analog_data_16bit(&msg,
								  &sensor_id_after,
								  &sensor_data_x_after,
								  &sensor_data_y_after,
								  &sensor_data_z_after);

		rockettest_check_expr_true(msg_is_sensor_data_after == true);
		rockettest_check_expr_true(type_after == MSG_SENSOR_DEM_ANALOG16);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(sensor_id_after == sensor_id_before);
		rockettest_check_expr_true(sensor_data_x_after == sensor_data_x_before);
		rockettest_check_expr_true(sensor_data_y_after == sensor_data_y_before);
		rockettest_check_expr_true(sensor_data_z_after == sensor_data_z_before);

		return test_passed;
	}
};

analog_sensor_dem_16bit_message_test analog_sensor_dem_16bit_message_test_inst;