#ifndef CANLIB_SENSOR_H
#define CANLIB_SENSOR_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

/*
 * Used to send tempurature measurement data
 * Units are 1/1024th of a degree C
 * Temp is 24 bit signed, not 32 bit. Values less than -8388608 or greater than
 * 8388607 will overflow.
 */
bool build_temp_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, uint8_t sensor_num, int32_t temp, can_msg_t *output
);

/*
 * Used to send altitude recived from altimiters
 */
bool build_altitude_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, int32_t altitude, can_msg_t *output
);

/*
 * Used to send 16-bit IMU data values.
 */
bool build_imu_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, char axis, can_imu_id_t imu_id, uint16_t linear_accel,
    uint16_t angular_velocity, can_msg_t *output
);

/*
 * Used to send 16-bit IMU magnetometer values.
 */
bool build_mag_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, char axis, can_imu_id_t imu_id, uint16_t mag_value,
    can_msg_t *output
);

/*
 * Used to build barometer raw data message
 */
bool build_baro_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, can_imu_id_t imu_id, uint32_t pressure, uint16_t temp,
    can_msg_t *output
);

/*
 * Used to send analog sensor data. The units of the sensor data are
 * not nailed down at this point and will likely differ based on the
 * sensor id.
 */
bool build_analog_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, can_analog_sensor_id_t sensor_id, uint16_t sensor_data,
    can_msg_t *output
);

bool is_sensor_data(const can_msg_t *msg);

/*
 * Gets the temp data and sensor num, returns false if the message is not
 * a SENSOR_TEMP message.
 */
bool get_temp_data(const can_msg_t *msg, uint8_t *sensor_num, int32_t *temp);

/*
 * Gets the altitude data, returns false if the message is not
 * a SENSOR_ALTITUDE message.
 */
bool get_altitude_data(const can_msg_t *msg, int32_t *altitude);

bool get_imu_mag_id_dimension(const can_msg_t *msg, can_imu_id_t *imu_id, char *dimension);

/*
 * Gets IMU data from an IMU message. Returns true if
 * successful, false if the input is invalid.
 */
bool get_imu_data(const can_msg_t *msg, uint16_t *linear_accel, uint16_t *angular_velocity);

bool get_mag_data(const can_msg_t *msg, uint16_t *mag_value);

/*
 * Gets analog data (the sensor ID and data itself) from an
 * analog message. Returns true if successful, false if
 * the input is invalid.
 */
bool get_analog_data(
    const can_msg_t *msg, can_analog_sensor_id_t *sensor_id, uint16_t *output_data
);

#endif
