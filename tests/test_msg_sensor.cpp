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

		std::uint8_t sensor_num_extracted;
		std::int32_t temp_extracted;

		sensor_num_extracted = msg.data[2];
		temp_extracted =
			(msg.data[3] << 24) | (msg.data[4] << 16) | (msg.data[5] << 8) | msg.data[6];

		rockettest_assert(msg.data_len == 7);
		rockettest_assert(sensor_num_extracted == sensor_num_before);
		rockettest_assert(temp_extracted == temp_before);

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

		std::int32_t altitude_extracted;
		can_apogee_state_t apogee_state_extracted;

		altitude_extracted =
			(msg.data[2] << 24) | (msg.data[3] << 16) | (msg.data[4] << 8) | msg.data[5];
		apogee_state_extracted = static_cast<can_apogee_state_t>(msg.data[6]);

		rockettest_assert(altitude_extracted == altitude_before);
		rockettest_assert(apogee_state_extracted == apogee_state_before);
		rockettest_assert(msg.data_len == 7);

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

		can_analog_sensor_id_t sensor_id_extracted;
		std::uint16_t sensor_data_extracted;

		sensor_id_extracted = static_cast<can_analog_sensor_id_t>(msg.data[2]);
		sensor_data_extracted = (msg.data[3] << 8) | msg.data[4];

		rockettest_assert(msg.data_len == 5);
		rockettest_assert(sensor_id_extracted == sensor_id_before);
		rockettest_assert(sensor_data_extracted == sensor_data_before);

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
