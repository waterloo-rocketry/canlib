#ifndef CANLIB_SENSOR_H
#define CANLIB_SENSOR_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void build_analog_data_16bit_msg(can_msg_prio_t prio, uint16_t timestamp,
								 can_analog_sensor_id_t sensor_id, uint16_t sensor_data,
								 can_msg_t *output);

void build_analog_data_32bit_msg(can_msg_prio_t prio, uint16_t timestamp,
								 can_analog_sensor_id_t sensor_id, uint32_t sensor_data,
								 can_msg_t *output);

void build_dem_analog_data_16bit_msg(can_msg_prio_t prio, uint16_t timestamp,
									 can_dem_sensor_id_t dem_sensor_id, uint16_t sensor_data_x,
									 uint16_t sensor_data_y, uint16_t sensor_data_z,
									 can_msg_t *output);

bool msg_is_sensor_data(const can_msg_t *msg);

bool get_analog_data_16bit(const can_msg_t *msg, can_analog_sensor_id_t *sensor_id,
						   uint16_t *output_data);

bool get_analog_data_32bit(const can_msg_t *msg, can_analog_sensor_id_t *sensor_id,
						   uint32_t *output_data);

bool get_dem_analog_data_16bit(const can_msg_t *msg, can_dem_sensor_id_t *dem_sensor_id,
							   uint16_t *output_data_x, uint16_t *output_data_y,
							   uint16_t *output_data_z);

#ifdef __cplusplus
}
#endif

#endif
