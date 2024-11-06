#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "common.h"
#include "message_types.h"
#include "msg_sensor.h"

bool build_imu_data_msg(
    enum MESSAGE_PRIO prio, uint16_t message_type,
    uint32_t timestamp, // acc, gyro, mag
    const uint16_t *imu_data, // x, y, z
    can_msg_t *output
) {
    if (!output) {
        return false;
    }
    if (!imu_data) {
        return false;
    }
    if (!(message_type == MSG_SENSOR_ACC || message_type == MSG_SENSOR_GYRO ||
          message_type == MSG_SENSOR_MAG || message_type == MSG_SENSOR_ACC2)) {
        return false;
    }

    output->sid = SID(prio, message_type);
    write_timestamp_2bytes(timestamp, output);

    // X value
    output->data[2] = (imu_data[0] >> 8) & 0xff;
    output->data[3] = (imu_data[0] >> 0) & 0xff;

    // Y value
    output->data[4] = (imu_data[1] >> 8) & 0xff;
    output->data[5] = (imu_data[1] >> 0) & 0xff;

    // Z value
    output->data[6] = (imu_data[2] >> 8) & 0xff;
    output->data[7] = (imu_data[2] >> 0) & 0xff;

    // this message type uses the entire data field
    output->data_len = 8;

    return true;
}

bool build_analog_data_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, enum SENSOR_ID sensor_id, uint16_t sensor_data,
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

bool build_temp_data_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, uint8_t sensor_num, int32_t temp, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_SENSOR_TEMP);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = sensor_num;
    output->data[4] = (temp >> 16) & 0xFF;
    output->data[5] = (temp >> 8) & 0xFF;
    output->data[6] = temp & 0xFF;
    output->data_len = 7;

    return true;
}

bool build_altitude_data_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, int32_t altitude, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_SENSOR_ALTITUDE);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = (altitude >> 24) & 0xFF;
    output->data[4] = (altitude >> 16) & 0xFF;
    output->data[5] = (altitude >> 8) & 0xFF;
    output->data[6] = altitude & 0xFF;
    output->data_len = 7;

    return true;
}

bool is_sensor_data(const can_msg_t *msg) {
    if (!msg) {
        return false;
    }

    uint16_t type = get_message_type(msg);
    if (type == MSG_SENSOR_ACC || type == MSG_SENSOR_ACC2 || type == MSG_SENSOR_GYRO ||
        type == MSG_SENSOR_MAG || type == MSG_SENSOR_ANALOG) {
        return true;
    } else {
        return false;
    }
}

bool get_imu_data(
    const can_msg_t *msg, uint16_t *output_x, uint16_t *output_y, uint16_t *output_z
) {
    if (!msg) {
        return false;
    }
    if (!output_x) {
        return false;
    }
    if (!output_y) {
        return false;
    }
    if (!output_z) {
        return false;
    }
    if (!is_sensor_data(msg)) {
        return false;
    }
    if (get_message_type(msg) == MSG_SENSOR_ANALOG) {
        return false;
    }

    *output_x = (uint16_t)msg->data[2] << 8 | msg->data[3]; // x
    *output_y = (uint16_t)msg->data[4] << 8 | msg->data[5]; // y
    *output_z = (uint16_t)msg->data[6] << 8 | msg->data[7]; // z

    return true;
}

bool get_analog_data(const can_msg_t *msg, enum SENSOR_ID *sensor_id, uint16_t *output_data) {
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

bool get_temp_data(const can_msg_t *msg, uint8_t *sensor_num, int32_t *temp) {
    if (!msg || !sensor_num || !temp) {
        return false;
    }
    if (get_message_type(msg) != MSG_SENSOR_TEMP) {
        return false;
    }

    *sensor_num = msg->data[3];
    // handle 24 bit to 32 bit sign conversion
    *temp = (msg->data[4] & 0x80) ? 0xFF << 24 : 0;
    *temp |= ((uint32_t)msg->data[4] << 16);
    *temp |= ((uint32_t)msg->data[5] << 8);
    *temp |= msg->data[6];

    return true;
}

bool get_altitude_data(const can_msg_t *msg, int32_t *altitude) {
    if (!msg || !altitude) {
        return false;
    }
    if (get_message_type(msg) != MSG_SENSOR_ALTITUDE) {
        return false;
    }

    *altitude = ((uint32_t)msg->data[3] << 24);
    *altitude |= ((uint32_t)msg->data[4] << 16);
    *altitude |= ((uint32_t)msg->data[5] << 8);
    *altitude |= msg->data[6];

    return true;
}

bool build_fill_msg(
    enum MESSAGE_PRIO prio, uint32_t timestamp, uint8_t lvl, uint8_t direction, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_FILL_LVL);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = lvl;
    output->data[4] = direction;

    output->data_len = 5;

    return true;
}

bool get_fill_info(const can_msg_t *msg, uint8_t *lvl, uint8_t *direction) {
    if ((!msg) | (!lvl) | (!direction)) {
        return false;
    }

    uint16_t msg_type = get_message_type(msg);
    if (msg_type == MSG_FILL_LVL) {
        *lvl = msg->data[3];
        *direction = msg->data[4];
        return true;
    }

    return false;
}
