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

		rockettest_assert(msg.data_len == 4);
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
		get_analog_data_16bit(&msg, &sensor_id_after, &sensor_data_after);

		rockettest_assert(msg_is_sensor_data_after == true);
		rockettest_assert(type_after == MSG_SENSOR_ANALOG16);
		rockettest_assert(timestamp_after == timestamp_before);
		rockettest_assert(sensor_id_after == sensor_id_before);
		rockettest_assert(sensor_data_after == sensor_data_before);

		return test_passed;
	}
};

analog_sensor_16bit_message_test analog_sensor_16bit_message_test_inst;
