#include <cstdint>

#include "test_common.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_sensor.h"
#include "message_types.h"

void test_analog_sensor_msg() {
	can_msg_t msg;

	can_msg_prio_t prio_before = test_rand<can_msg_prio_t, 0x3>();
	std::uint16_t timestamp_before = test_rand<std::uint16_t>();
	can_analog_sensor_id_t sensor_id_before = test_rand<can_analog_sensor_id_t, 0xff>();
	std::uint16_t sensor_data_before = test_rand<std::uint16_t>();

	build_analog_data_msg(
		prio_before, timestamp_before, sensor_id_before, sensor_data_before, &msg);

	can_msg_type_t type_after;
	std::uint16_t timestamp_after;
	can_analog_sensor_id_t sensor_id_after;
	std::uint16_t sensor_data_after;

	type_after = get_message_type(&msg);
	timestamp_after = get_timestamp(&msg);
	get_analog_data(&msg, &sensor_id_after, &sensor_data_after);

	test_assert(type_after == MSG_SENSOR_ANALOG);
	test_assert(timestamp_after == timestamp_before);
	test_assert(sensor_id_after == sensor_id_before);
	test_assert(sensor_data_after == sensor_data_before);
}
