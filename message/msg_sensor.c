#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_sensor.h"

void build_analog_data_16bit_msg(can_msg_prio_t prio, uint16_t timestamp,
								 can_analog_sensor_id_t sensor_id, uint16_t sensor_data,
								 can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_SENSOR_ANALOG16, sensor_id);
	write_timestamp(timestamp, output);

	output->data[2] = (sensor_data >> 8) & 0xff;
	output->data[3] = (sensor_data >> 0) & 0xff;

	output->data_len = 4;
}

void build_analog_data_32bit_msg(can_msg_prio_t prio, uint16_t timestamp,
								 can_analog_sensor_id_t sensor_id, uint32_t sensor_data,
								 can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_SENSOR_ANALOG32, sensor_id);
	write_timestamp(timestamp, output);

	output->data[2] = (sensor_data >> 24) & 0xff;
	output->data[3] = (sensor_data >> 16) & 0xff;
	output->data[4] = (sensor_data >> 8) & 0xff;
	output->data[5] = (sensor_data >> 0) & 0xff;

	output->data_len = 6;
}

void build_dem_analog_data_16bit_msg(can_msg_prio_t prio, uint16_t timestamp,
									 can_dem_sensor_id_t dem_sensor_id, uint16_t sensor_data_x,
									 uint16_t sensor_data_y, uint16_t sensor_data_z,
									 can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_SENSOR_DEM_ANALOG16, dem_sensor_id);
	write_timestamp(timestamp, output);

	output->data[2] = (sensor_data_x >> 8) & 0xff;
	output->data[3] = (sensor_data_x >> 0) & 0xff;
	output->data[4] = (sensor_data_y >> 8) & 0xff;
	output->data[5] = (sensor_data_y >> 0) & 0xff;
	output->data[6] = (sensor_data_z >> 8) & 0xff;
	output->data[7] = (sensor_data_z >> 0) & 0xff;

	output->data_len = 8;
}

bool msg_is_sensor_data(const can_msg_t *msg) {
	if (!msg) {
		return false;
	}

	uint16_t type = get_message_type(msg);
	if (type == MSG_SENSOR_ANALOG16 || type == MSG_SENSOR_ANALOG32 ||
		type == MSG_SENSOR_DEM_ANALOG16) {
		return true;
	} else {
		return false;
	}
}

bool get_analog_data_16bit(const can_msg_t *msg, can_analog_sensor_id_t *sensor_id,
						   uint16_t *output_data) {
	if (!msg) {
		return false;
	}
	if (!output_data) {
		return false;
	}
	if (get_message_type(msg) != MSG_SENSOR_ANALOG16) {
		return false;
	}

	*sensor_id = get_message_metadata(msg);
	*output_data = ((uint16_t)msg->data[2] << 8) | msg->data[3];

	return true;
}

bool get_analog_data_32bit(const can_msg_t *msg, can_analog_sensor_id_t *sensor_id,
						   uint32_t *output_data) {
	if (!msg) {
		return false;
	}
	if (!output_data) {
		return false;
	}
	if (get_message_type(msg) != MSG_SENSOR_ANALOG32) {
		return false;
	}

	*sensor_id = get_message_metadata(msg);
	*output_data = ((uint32_t)msg->data[2] << 24) | ((uint32_t)msg->data[3] << 16) |
				   ((uint32_t)msg->data[4] << 8) | msg->data[5];

	return true;
}

bool get_dem_analog_data_16bit(const can_msg_t *msg, can_dem_sensor_id_t *dem_sensor_id,
							   uint16_t *output_data_x, uint16_t *output_data_y,
							   uint16_t *output_data_z) {
	if (!msg) {
		return false;
	}
	if (!output_data_x) {
		return false;
	}
	if (get_message_type(msg) != MSG_SENSOR_ANALOG16) {
		return false;
	}

	*dem_sensor_id = get_message_metadata(msg);
	*output_data_x = ((uint16_t)msg->data[2] << 8) | msg->data[3];
	*output_data_y = ((uint16_t)msg->data[4] << 8) | msg->data[5];
	*output_data_z = ((uint16_t)msg->data[6] << 8) | msg->data[7];

	return true;
}
