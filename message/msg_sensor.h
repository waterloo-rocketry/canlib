#ifndef CANLIB_SENSOR_H
#define CANLIB_SENSOR_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

/*
 * Used to send analog sensor data. The units of the sensor data are
 * not nailed down at this point and will likely differ based on the
 * sensor id.
 */
bool build_analog_data_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, enum SENSOR_ID sensor_id, uint16_t sensor_data,
    can_msg_t *output
);

/*
 * Used to send 16-bit IMU data values. It is assumed that an array
 * of 3 values is sent (X, Y, and Z axes).
 */
bool build_imu_data_msg(
    enum MESSAGE_PRIO prio, uint16_t message_type, // acc, gyro, mag
    uint32_t timestamp,
    const uint16_t *imu_data, // x, y, z
    can_msg_t *output
);

/*
 * Used to send tempurature measurement data
 * Units are 1/1024th of a degree C
 * Temp is 24 bit signed, not 32 bit. Values less than -8388608 or greater than
 * 8388607 will overflow.
 */
bool build_temp_data_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, uint8_t sensor_num, int32_t temp, can_msg_t *output
);

/*
 * Used to send altitude recived from altimiters
 */
bool build_altitude_data_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, int32_t altitude, can_msg_t *output
);

/*
 * Returns true if msg is of type SENSOR_ACC, SENSOR_GYRO, SENSOR_MAG,
 * or SENSOR_ANALOG. Otherwise it returns false
 */
bool is_sensor_data(const can_msg_t *msg);

/*
 * Gets IMU data from an IMU message. Returns true if
 * successful, false if the input is invalid.
 */
bool get_imu_data(const can_msg_t *msg, uint16_t *output_x, uint16_t *output_y, uint16_t *output_z);

/*
 * Gets analog data (the sensor ID and data itself) from an
 * analog message. Returns true if successful, false if
 * the input is invalid.
 */
bool get_analog_data(const can_msg_t *msg, enum SENSOR_ID *sensor_id, uint16_t *output_data);

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

/*
 * Used to send fill sensing data. Currently senses fill level
 * measured by sensor number, and direction of fill travel.
 */
bool build_fill_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, uint8_t lvl, uint8_t direction, can_msg_t *output
);

/*
 * Populates provided lvl and direction arguments with data unpacked
 * from fill sensing message.
 */
bool get_fill_info(const can_msg_t *msg, uint8_t *lvl, uint8_t *direction);

#endif
