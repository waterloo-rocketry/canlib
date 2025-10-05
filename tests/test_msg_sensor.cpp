#include <cstdint>

#include "rockettest.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_sensor.h"
#include "message_types.h"

class temp_data_msg_test : rockettest_test {
public:
	temp_data_msg_test() : rockettest_test("temp_data_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		std::uint8_t sensor_num_before = rockettest_rand<std::uint8_t>();
		std::int32_t temp_before = rockettest_rand<std::int32_t>();

		build_temp_data_msg(prio_before, timestamp_before, sensor_num_before, temp_before, &msg);

		bool is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		std::uint8_t sensor_num_after;
		std::int32_t temp_after;

		is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_temp_data(&msg, &sensor_num_after, &temp_after);

		rockettest_assert(is_sensor_data_after == true);
		rockettest_assert(type_after == MSG_SENSOR_TEMP);
		rockettest_assert(timestamp_after == timestamp_before);
		rockettest_assert(sensor_num_after == sensor_num_before);
		rockettest_assert(temp_after == temp_before);

		return test_passed;
	}
};

temp_data_msg_test temp_data_msg_test_inst;

class altitude_data_msg_test : rockettest_test {
public:
	altitude_data_msg_test() : rockettest_test("altitude_data_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		std::int32_t altitude_before = rockettest_rand<std::int32_t>();
		can_apogee_state_t apogee_state_before = rockettest_rand<can_apogee_state_t, 0xff>();

		build_altitude_data_msg(
			prio_before, timestamp_before, altitude_before, apogee_state_before, &msg);

		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		std::int32_t altitude_after;
		can_apogee_state_t apogee_state_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_altitude_data(&msg, &altitude_after, &apogee_state_after);

		rockettest_assert(msg_is_sensor_data_after == true);
		rockettest_assert(type_after == MSG_SENSOR_ALTITUDE);
		rockettest_assert(timestamp_after == timestamp_before);
		rockettest_assert(altitude_after == altitude_before);
		rockettest_assert(apogee_state_after == apogee_state_before);

		return test_passed;
	}
};

altitude_data_msg_test altitude_data_msg_test_inst;

class analog_sensor_message_test : rockettest_test {
public:
	analog_sensor_message_test() : rockettest_test("analog_sensor_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		can_analog_sensor_id_t sensor_id_before = rockettest_rand<can_analog_sensor_id_t, 0xff>();
		std::uint16_t sensor_data_before = rockettest_rand<std::uint16_t>();

		build_analog_data_msg(
			prio_before, timestamp_before, sensor_id_before, sensor_data_before, &msg);

		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_analog_sensor_id_t sensor_id_after;
		std::uint16_t sensor_data_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_analog_data(&msg, &sensor_id_after, &sensor_data_after);

		rockettest_assert(msg_is_sensor_data_after == true);
		rockettest_assert(type_after == MSG_SENSOR_ANALOG);
		rockettest_assert(timestamp_after == timestamp_before);
		rockettest_assert(sensor_id_after == sensor_id_before);
		rockettest_assert(sensor_data_after == sensor_data_before);

		return test_passed;
	}
};

analog_sensor_message_test analog_sensor_message_test_inst;

class imu_data_msg_test : rockettest_test {
public:
	imu_data_msg_test() : rockettest_test("imu_data_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		char axis_before = 'X';
		can_imu_id_t imu_id_before = rockettest_rand<can_imu_id_t, IMU_ENUM_MAX - 1>();
		std::uint16_t linear_accel_before = rockettest_rand<std::uint16_t>();
		std::uint16_t angular_velocity_before = rockettest_rand<std::uint16_t>();

		build_imu_data_msg(prio_before, timestamp_before, axis_before, imu_id_before,
						   linear_accel_before, angular_velocity_before, &msg);

		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_imu_id_t imu_id_after;
		char axis_after;
		std::uint16_t linear_accel_after;
		std::uint16_t angular_velocity_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_imu_mag_id_dimension(&msg, &imu_id_after, &axis_after);
		get_imu_data(&msg, &linear_accel_after, &angular_velocity_after);

		rockettest_assert(msg_is_sensor_data_after == true);
		rockettest_assert(type_after == MSG_SENSOR_IMU_X);
		rockettest_assert(timestamp_after == timestamp_before);
		rockettest_assert(imu_id_after == imu_id_before);
		rockettest_assert(axis_after == axis_before);
		rockettest_assert(linear_accel_after == linear_accel_before);
		rockettest_assert(angular_velocity_after == angular_velocity_before);

		return test_passed;
	}
};

imu_data_msg_test imu_data_msg_test_inst;

class mag_data_msg_test : rockettest_test {
public:
	mag_data_msg_test() : rockettest_test("mag_data_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		char axis_before = 'Y';
		can_imu_id_t imu_id_before = rockettest_rand<can_imu_id_t, IMU_ENUM_MAX - 1>();
		std::uint16_t mag_value_before = rockettest_rand<std::uint16_t>();

		build_mag_data_msg(prio_before, timestamp_before, axis_before, imu_id_before,
						   mag_value_before, &msg);

		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_imu_id_t imu_id_after;
		char axis_after;
		std::uint16_t mag_value_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_imu_mag_id_dimension(&msg, &imu_id_after, &axis_after);
		get_mag_data(&msg, &mag_value_after);

		rockettest_assert(msg_is_sensor_data_after == true);
		rockettest_assert(type_after == MSG_SENSOR_MAG_Y);
		rockettest_assert(timestamp_after == timestamp_before);
		rockettest_assert(imu_id_after == imu_id_before);
		rockettest_assert(axis_after == axis_before);
		rockettest_assert(mag_value_after == mag_value_before);

		return test_passed;
	}
};

mag_data_msg_test mag_data_msg_test_inst;

class baro_data_msg_test : rockettest_test {
public:
	baro_data_msg_test() : rockettest_test("baro_data_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		can_imu_id_t imu_id_before = rockettest_rand<can_imu_id_t, IMU_ENUM_MAX - 1>();
		std::uint32_t pressure_before = rockettest_rand<std::uint32_t, 0xFFFFFF>(); // 24-bit
		std::uint16_t temp_before = rockettest_rand<std::uint16_t>();

		build_baro_data_msg(prio_before, timestamp_before, imu_id_before, pressure_before,
							temp_before, &msg);

		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_imu_id_t imu_id_after;
		std::uint32_t pressure_after;
		std::uint16_t temp_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_baro_data(&msg, &imu_id_after, &pressure_after, &temp_after);

		rockettest_assert(msg_is_sensor_data_after == true);
		rockettest_assert(type_after == MSG_SENSOR_BARO);
		rockettest_assert(timestamp_after == timestamp_before);
		rockettest_assert(imu_id_after == imu_id_before);
		rockettest_assert(pressure_after == pressure_before);
		rockettest_assert(temp_after == temp_before);

		return test_passed;
	}
};

baro_data_msg_test baro_data_msg_test_inst;
