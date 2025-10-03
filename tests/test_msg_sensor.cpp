#include <cstdint>

#include "test_common.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_sensor.h"
#include "message_types.h"

class temp_data_msg_test : rocketry_test {
public:
	const char *get_name() override {
		return "temp_data_msg_test";
	}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = test_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = test_rand<std::uint16_t>();
		std::uint8_t sensor_num_before = test_rand<std::uint8_t>();
		std::int32_t temp_before = test_rand<std::int32_t>();

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

		test_assert(is_sensor_data_after == true);
		test_assert(type_after == MSG_SENSOR_TEMP);
		test_assert(timestamp_after == timestamp_before);
		test_assert(sensor_num_after == sensor_num_before);
		test_assert(temp_after == temp_before);

		return test_passed;
	}
};

temp_data_msg_test temp_data_msg_test_inst;

class altitude_data_msg_test : rocketry_test {
public:
	const char *get_name() override {
		return "altitide_data_msg_test";
	}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = test_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = test_rand<std::uint16_t>();
		std::int32_t altitude_before = test_rand<std::int32_t>();
		can_apogee_state_t apogee_state_before = test_rand<can_apogee_state_t, 0xff>();

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

		test_assert(msg_is_sensor_data_after == true);
		test_assert(type_after == MSG_SENSOR_ALTITUDE);
		test_assert(timestamp_after == timestamp_before);
		test_assert(altitude_after == altitude_before);
		test_assert(apogee_state_after == apogee_state_before);

		return test_passed;
	}
};

altitude_data_msg_test altitude_data_msg_test_inst;

class analog_sensor_message_test : rocketry_test {
public:
	const char *get_name() override {
		return "analog_sensor_msg_test";
	}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = test_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = test_rand<std::uint16_t>();
		can_analog_sensor_id_t sensor_id_before = test_rand<can_analog_sensor_id_t, 0xff>();
		std::uint16_t sensor_data_before = test_rand<std::uint16_t>();

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

		test_assert(msg_is_sensor_data_after == true);
		test_assert(type_after == MSG_SENSOR_ANALOG);
		test_assert(timestamp_after == timestamp_before);
		test_assert(sensor_id_after == sensor_id_before);
		test_assert(sensor_data_after == sensor_data_before);

		test_assert(false);

		return test_passed;
	}
};

analog_sensor_message_test analog_sensor_message_test_inst;
