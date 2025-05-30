#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_sensor.h"

bool build_temp_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, uint8_t sensor_num, int32_t temp, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_SENSOR_TEMP);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = sensor_num;
    output->data[3] = (temp >> 24) & 0xFF;
    output->data[4] = (temp >> 16) & 0xFF;
    output->data[5] = (temp >> 8) & 0xFF;
    output->data[6] = temp & 0xFF;
    output->data_len = 7;

    return true;
}

bool build_altitude_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, int32_t altitude, can_apogee_state_t apogee_state,
    can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_SENSOR_ALTITUDE);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = (altitude >> 24) & 0xFF;
    output->data[3] = (altitude >> 16) & 0xFF;
    output->data[4] = (altitude >> 8) & 0xFF;
    output->data[5] = altitude & 0xFF;
    output->data[6] = apogee_state;
    output->data_len = 7;

    return true;
}

bool build_imu_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, char axis, can_imu_id_t imu_id, uint16_t linear_accel,
    uint16_t angular_velocity, can_msg_t *output
) {
    if (!output) {
        return false;
    }
    if (axis == 'X') {
        output->sid = SID(prio, MSG_SENSOR_IMU_X);
    } else if (axis == 'Y') {
        output->sid = SID(prio, MSG_SENSOR_IMU_Y);
    } else {
        output->sid = SID(prio, MSG_SENSOR_IMU_Z);
    }

    write_timestamp_2bytes(timestamp, output);

    output->data[2] = imu_id;
    output->data[3] = (linear_accel >> 8) & 0xff;
    output->data[4] = (linear_accel >> 0) & 0xff;
    output->data[5] = (angular_velocity >> 8) & 0xff;
    output->data[6] = (angular_velocity >> 0) & 0xff;
    output->data_len = 7;

    return true;
}

bool build_mag_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, char axis, can_imu_id_t imu_id, uint16_t mag_value,
    can_msg_t *output
) {
    if (!output) {
        return false;
    }
    if (axis == 'X') {
        output->sid = SID(prio, MSG_SENSOR_MAG_X);
    } else if (axis == 'Y') {
        output->sid = SID(prio, MSG_SENSOR_MAG_Y);
    } else {
        output->sid = SID(prio, MSG_SENSOR_MAG_Z);
    }

    write_timestamp_2bytes(timestamp, output);

    output->data[2] = imu_id;
    output->data[3] = (mag_value >> 8) & 0xff;
    output->data[4] = (mag_value >> 0) & 0xff;
    output->data_len = 5;

    return true;
}

bool build_baro_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, can_imu_id_t imu_id, uint32_t pressure, uint16_t temp,
    can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_SENSOR_BARO);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = imu_id;
    output->data[3] = (pressure >> 16) & 0xFF;
    output->data[4] = (pressure >> 8) & 0xFF;
    output->data[5] = pressure & 0xFF;
    output->data[6] = (temp >> 8) & 0xFF;
    output->data[7] = temp & 0xFF;

    output->data_len = 8;

    return true;
}

bool build_analog_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, can_analog_sensor_id_t sensor_id, uint16_t sensor_data,
    can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_SENSOR_ANALOG);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = (uint8_t)sensor_id;
    output->data[3] = (sensor_data >> 8) & 0xff;
    output->data[4] = (sensor_data >> 0) & 0xff;

    output->data_len = 5;

    return true;
}

bool is_sensor_data(const can_msg_t *msg) {
    if (!msg) {
        return false;
    }

    uint16_t type = get_message_type(msg);
    if (type == MSG_SENSOR_TEMP || type == MSG_SENSOR_ALTITUDE || type == MSG_SENSOR_IMU_X ||
        type == MSG_SENSOR_IMU_Y || type == MSG_SENSOR_IMU_Z || type == MSG_SENSOR_MAG_Z ||
        type == MSG_SENSOR_MAG_Y || type == MSG_SENSOR_MAG_Z || type == MSG_SENSOR_BARO ||
        type == MSG_SENSOR_ANALOG) {
        return true;
    } else {
        return false;
    }
}

bool get_temp_data(const can_msg_t *msg, uint8_t *sensor_num, int32_t *temp) {
    if (!msg || !sensor_num || !temp) {
        return false;
    }
    if (get_message_type(msg) != MSG_SENSOR_TEMP) {
        return false;
    }

    *sensor_num = msg->data[2];
    *temp = ((uint32_t)msg->data[3] << 24);
    *temp |= ((uint32_t)msg->data[4] << 16);
    *temp |= ((uint32_t)msg->data[5] << 8);
    *temp |= msg->data[6];

    return true;
}

bool get_altitude_data(const can_msg_t *msg, int32_t *altitude, can_apogee_state_t *apogee_state) {
    if (!msg || !altitude) {
        return false;
    }
    if (get_message_type(msg) != MSG_SENSOR_ALTITUDE) {
        return false;
    }

    *altitude = ((uint32_t)msg->data[2] << 24);
    *altitude |= ((uint32_t)msg->data[3] << 16);
    *altitude |= ((uint32_t)msg->data[4] << 8);
    *altitude |= msg->data[5];

    *apogee_state = msg->data[6];

    return true;
}

bool get_imu_mag_id_dimension(const can_msg_t *msg, can_imu_id_t *imu_id, char *dimension) {
    if (!msg || !imu_id || !dimension) {
        return false;
    }

    can_msg_type_t msg_type = get_message_type(msg);
    if ((msg_type == MSG_SENSOR_IMU_X) || (msg_type == MSG_SENSOR_MAG_X)) {
        *dimension = 'X';
    } else if ((msg_type == MSG_SENSOR_IMU_Y) || (msg_type == MSG_SENSOR_MAG_Y)) {
        *dimension = 'Y';
    } else if ((msg_type == MSG_SENSOR_IMU_Z) || (msg_type == MSG_SENSOR_MAG_Z)) {
        *dimension = 'Z';
    } else {
        return false;
    }

    *imu_id = msg->data[2];
    return true;
}

bool get_imu_data(const can_msg_t *msg, uint16_t *linear_accel, uint16_t *angular_velocity) {
    if (!msg) {
        return false;
    }
    if (!linear_accel) {
        return false;
    }
    if (!angular_velocity) {
        return false;
    }

    *linear_accel = (uint16_t)msg->data[3] << 8 | msg->data[4];
    *angular_velocity = (uint16_t)msg->data[5] << 8 | msg->data[6];

    return true;
}

bool get_mag_data(const can_msg_t *msg, uint16_t *mag_value) {
    if (!msg) {
        return false;
    }
    if (!mag_value) {
        return false;
    }

    *mag_value = (uint16_t)msg->data[3] << 8 | msg->data[4];

    return true;
}

bool get_baro_data(const can_msg_t *msg, can_imu_id_t *imu_id, uint32_t *pressure, uint16_t *temp) {
    if (!msg) {
        return false;
    }
    if (!pressure) {
        return false;
    }
    if (!temp) {
        return false;
    }

    *imu_id = msg->data[2];
    *pressure = ((uint32_t)msg->data[3] << 16);
    *pressure |= ((uint32_t)msg->data[4] << 8);
    *pressure |= msg->data[5];
    *temp = (uint16_t)msg->data[6] << 8 | msg->data[7];

    return true;
}

bool get_analog_data(
    const can_msg_t *msg, can_analog_sensor_id_t *sensor_id, uint16_t *output_data
) {
    if (!msg) {
        return false;
    }
    if (!output_data) {
        return false;
    }
    if (get_message_type(msg) != MSG_SENSOR_ANALOG) {
        return false;
    }

    *sensor_id = msg->data[2];
    *output_data = (uint16_t)msg->data[3] << 8 | msg->data[4];

    return true;
}

