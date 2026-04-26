#include <cstdint>

#include "rockettest.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_sensor.h"
#include "message_types.h"

namespace {

	constexpr std::uint32_t kMax24BitValue = (1u << 24) - 1;

}

class analog_sensor_16bit_message_test : rockettest_test {
public:
	analog_sensor_16bit_message_test() : rockettest_test("analog_sensor_16bit_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		can_analog_sensor_id_t sensor_id_before =
			rockettest_rand_field<can_analog_sensor_id_t, 0xff>();
		std::uint16_t sensor_data_before = rockettest_rand_field<std::uint16_t>();

		build_analog_sensor_16bit_msg(
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

		msg_is_sensor_data_after = msg_is_analog_sensor(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_analog_sensor_data_16bit(&msg, &sensor_id_after, &sensor_data_after);

		rockettest_check_expr_true(msg_is_sensor_data_after == true);
		rockettest_check_expr_true(type_after == MSG_SENSOR_ANALOG16);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(sensor_id_after == sensor_id_before);
		rockettest_check_expr_true(sensor_data_after == sensor_data_before);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio_before, MSG_SENSOR_ANALOG32, sensor_id_before);
		w_status_t parse_status = get_analog_sensor_data_16bit(&invalid_type_msg, &sensor_id_after, &sensor_data_after);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 3;
		parse_status = get_analog_sensor_data_16bit(&invalid_len_msg, &sensor_id_after, &sensor_data_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);

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

		can_msg_prio_t prio_before = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		can_analog_sensor_id_t sensor_id_before =
			rockettest_rand_field<can_analog_sensor_id_t, 0xff>();
		std::uint32_t sensor_data_before =
			(static_cast<std::uint32_t>(rockettest_rand_field<uint16_t>()) << 16) |
			rockettest_rand_field<uint16_t>();

		build_analog_sensor_32bit_msg(
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

		msg_is_sensor_data_after = msg_is_analog_sensor(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_analog_sensor_data_32bit(&msg, &sensor_id_after, &sensor_data_after);

		rockettest_check_expr_true(msg_is_sensor_data_after == true);
		rockettest_check_expr_true(type_after == MSG_SENSOR_ANALOG32);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(sensor_id_after == sensor_id_before);
		rockettest_check_expr_true(sensor_data_after == sensor_data_before);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio_before, MSG_SENSOR_ANALOG16, sensor_id_before);
		w_status_t parse_status = get_analog_sensor_data_32bit(&invalid_type_msg, &sensor_id_after, &sensor_data_after);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 5;
		parse_status = get_analog_sensor_data_32bit(&invalid_len_msg, &sensor_id_after, &sensor_data_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

analog_sensor_32bit_message_test analog_sensor_32bit_message_test_inst;

class analog_sensor_dem_3d_16bit_message_test : rockettest_test {
public:
	analog_sensor_dem_3d_16bit_message_test()
		: rockettest_test("analog_sensor_dem_3d_16bit_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		can_dem_3d_sensor_id_t sensor_id_before =
			rockettest_rand_field<can_dem_3d_sensor_id_t, 0xff>();
		std::uint16_t sensor_data_x_before = rockettest_rand_field<std::uint16_t>();
		std::uint16_t sensor_data_y_before = rockettest_rand_field<std::uint16_t>();
		std::uint16_t sensor_data_z_before = rockettest_rand_field<std::uint16_t>();

		build_3d_analog_sensor_16bit_msg(prio_before,
										 timestamp_before,
										 sensor_id_before,
										 sensor_data_x_before,
										 sensor_data_y_before,
										 sensor_data_z_before,
										 &msg);

		can_dem_3d_sensor_id_t sensor_id_extracted;
		std::uint16_t sensor_data_x_extracted;
		std::uint16_t sensor_data_y_extracted;
		std::uint16_t sensor_data_z_extracted;

		sensor_id_extracted = static_cast<can_dem_3d_sensor_id_t>(msg.sid & 0xff);
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
		can_dem_3d_sensor_id_t sensor_id_after;
		std::uint16_t sensor_data_x_after;
		std::uint16_t sensor_data_y_after;
		std::uint16_t sensor_data_z_after;

		msg_is_sensor_data_after = msg_is_analog_sensor(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_3d_analog_sensor_data_16bit(&msg,
										&sensor_id_after,
										&sensor_data_x_after,
										&sensor_data_y_after,
										&sensor_data_z_after);

		rockettest_check_expr_true(msg_is_sensor_data_after == true);
		rockettest_check_expr_true(type_after == MSG_SENSOR_3D_ANALOG16);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(sensor_id_after == sensor_id_before);
		rockettest_check_expr_true(sensor_data_x_after == sensor_data_x_before);
		rockettest_check_expr_true(sensor_data_y_after == sensor_data_y_before);
		rockettest_check_expr_true(sensor_data_z_after == sensor_data_z_before);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio_before, MSG_SENSOR_2D_ANALOG24, sensor_id_before);
		w_status_t parse_status = get_3d_analog_sensor_data_16bit(&invalid_type_msg, &sensor_id_after, &sensor_data_x_after, &sensor_data_y_after, &sensor_data_z_after);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 7;
		parse_status = get_3d_analog_sensor_data_16bit(&invalid_len_msg, &sensor_id_after, &sensor_data_x_after, &sensor_data_y_after, &sensor_data_z_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

analog_sensor_dem_3d_16bit_message_test analog_sensor_dem_3d_16bit_message_test_inst;

class analog_sensor_dem_2d_24bit_message_test : rockettest_test {
public:
	analog_sensor_dem_2d_24bit_message_test()
		: rockettest_test("analog_sensor_dem_2d_24bit_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		can_dem_2d_sensor_id_t sensor_id_before =
			rockettest_rand_field<can_dem_2d_sensor_id_t, 0xff>();
		std::uint32_t sensor_data_x_before = rockettest_rand_field<std::uint32_t, kMax24BitValue>();
		std::uint32_t sensor_data_y_before = rockettest_rand_field<std::uint32_t, kMax24BitValue>();

		build_2d_analog_sensor_24bit_msg(prio_before,
										 timestamp_before,
										 sensor_id_before,
										 sensor_data_x_before,
										 sensor_data_y_before,
										 &msg);

		can_dem_2d_sensor_id_t sensor_id_extracted;
		std::uint32_t sensor_data_x_extracted;
		std::uint32_t sensor_data_y_extracted;

		sensor_id_extracted = static_cast<can_dem_2d_sensor_id_t>(msg.sid & 0xff);
		sensor_data_x_extracted =
			((uint32_t)msg.data[2] << 16) | ((uint32_t)msg.data[3] << 8) | msg.data[4];
		sensor_data_y_extracted =
			((uint32_t)msg.data[5] << 16) | ((uint32_t)msg.data[6] << 8) | msg.data[7];

		rockettest_check_expr_true(msg.data_len == 8);
		rockettest_check_expr_true(sensor_id_extracted == sensor_id_before);
		rockettest_check_expr_true(sensor_data_x_extracted == sensor_data_x_before);
		rockettest_check_expr_true(sensor_data_y_extracted == sensor_data_y_before);

		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_dem_2d_sensor_id_t sensor_id_after;
		std::uint32_t sensor_data_x_after;
		std::uint32_t sensor_data_y_after;

		msg_is_sensor_data_after = msg_is_analog_sensor(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_2d_analog_sensor_data_24bit(
			&msg, &sensor_id_after, &sensor_data_x_after, &sensor_data_y_after);

		rockettest_check_expr_true(msg_is_sensor_data_after == true);
		rockettest_check_expr_true(type_after == MSG_SENSOR_2D_ANALOG24);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(sensor_id_after == sensor_id_before);
		rockettest_check_expr_true(sensor_data_x_after == sensor_data_x_before);
		rockettest_check_expr_true(sensor_data_y_after == sensor_data_y_before);

		std::uint32_t invalid_sensor_data = kMax24BitValue + 1;
		rockettest_check_assert_triggered([=]() mutable {build_2d_analog_sensor_24bit_msg(prio_before, timestamp_before, sensor_id_before, invalid_sensor_data, sensor_data_y_before, &msg); });
		rockettest_check_assert_triggered([=]() mutable {build_2d_analog_sensor_24bit_msg(prio_before, timestamp_before, sensor_id_before, sensor_data_x_before, invalid_sensor_data, &msg); });

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio_before, MSG_SENSOR_ANALOG32, sensor_id_before);
		w_status_t parse_status = get_2d_analog_sensor_data_24bit(&invalid_type_msg, &sensor_id_after, &sensor_data_x_after, &sensor_data_y_after);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 7;
		parse_status = get_2d_analog_sensor_data_24bit(&invalid_len_msg, &sensor_id_after, &sensor_data_x_after, &sensor_data_y_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

analog_sensor_dem_2d_24bit_message_test analog_sensor_dem_2d_24bit_message_test_inst;

class analog_sensor_message_general_test : rockettest_test {
public:
	analog_sensor_message_general_test()
		: rockettest_test("analog_sensor_message_general_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp = rockettest_rand_field<std::uint16_t>();
		can_analog_sensor_id_t sensor_id =
			rockettest_rand_field<can_analog_sensor_id_t, 0xff>();
		std::uint16_t sensor_data = rockettest_rand_field<std::uint16_t>();

		build_analog_sensor_16bit_msg(
			prio, timestamp, sensor_id, sensor_data, &msg);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio, MSG_GENERAL_BOARD_STATUS, sensor_id);
		rockettest_check_expr_true(msg_is_analog_sensor(&invalid_type_msg) == false);

		return test_passed;
	}
};

analog_sensor_message_general_test analog_sensor_message_general_test_inst;