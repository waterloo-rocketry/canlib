#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_gps.h"

bool build_gps_time_msg(
    can_msg_prio_t prio, uint32_t timestamp, uint8_t utc_hours, uint8_t utc_mins, uint8_t utc_secs,
    uint8_t utc_dsecs, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_GPS_TIMESTAMP);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = utc_hours;
    output->data[4] = utc_mins;
    output->data[5] = utc_secs;
    output->data[6] = utc_dsecs;

    output->data_len = 7;

    return true;
}

bool build_gps_lat_msg(
    can_msg_prio_t prio, uint32_t timestamp, uint8_t degrees, uint8_t minutes, uint16_t dminutes,
    uint8_t direction, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_GPS_LATITUDE);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = degrees;
    output->data[4] = minutes;
    output->data[5] = dminutes >> 8;
    output->data[6] = dminutes & 0xFF;
    output->data[7] = direction;

    output->data_len = 8;

    return true;
}

bool build_gps_lon_msg(
    can_msg_prio_t prio, uint32_t timestamp, uint8_t degrees, uint8_t minutes, uint16_t dminutes,
    uint8_t direction, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_GPS_LONGITUDE);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = degrees;
    output->data[4] = minutes;
    output->data[5] = dminutes >> 8;
    output->data[6] = dminutes & 0xFF;
    output->data[7] = direction;

    output->data_len = 8;

    return true;
}

bool build_gps_alt_msg(
    can_msg_prio_t prio, uint32_t timestamp, uint16_t altitude, uint8_t daltitude, uint8_t units,
    can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_GPS_ALTITUDE);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = (altitude >> 8) & 0xff;
    output->data[4] = (altitude >> 0) & 0xff;
    output->data[5] = daltitude;
    output->data[6] = units;

    output->data_len = 7;

    return true;
}

bool build_gps_info_msg(
    can_msg_prio_t prio, uint32_t timestamp, uint8_t num_sat, uint8_t quality, can_msg_t *output
) {
    if (!output) {
        return false;
    }

    output->sid = SID(prio, MSG_GPS_INFO);
    write_timestamp_3bytes(timestamp, output);

    output->data[3] = num_sat;
    output->data[4] = quality;

    output->data_len = 5;

    return true;
}

bool get_gps_time(
    const can_msg_t *msg, uint8_t *utc_hours, uint8_t *utc_mins, uint8_t *utc_secs,
    uint8_t *utc_dsecs
) {
    if (!msg) {
        return false;
    }
    if (!utc_hours) {
        return false;
    }
    if (!utc_mins) {
        return false;
    }
    if (!utc_secs) {
        return false;
    }
    if (!utc_dsecs) {
        return false;
    }
    if (get_message_type(msg) != MSG_GPS_TIMESTAMP) {
        return false;
    }

    *utc_hours = msg->data[3];
    *utc_mins = msg->data[4];
    *utc_secs = msg->data[5];
    *utc_dsecs = msg->data[6];

    return true;
}

bool get_gps_lat(
    const can_msg_t *msg, uint8_t *degrees, uint8_t *minutes, uint16_t *dminutes, uint8_t *direction
) {
    if (!msg) {
        return false;
    }
    if (!degrees) {
        return false;
    }
    if (!minutes) {
        return false;
    }
    if (!dminutes) {
        return false;
    }
    if (!direction) {
        return false;
    }
    if (get_message_type(msg) != MSG_GPS_LATITUDE) {
        return false;
    }

    *degrees = msg->data[3];
    *minutes = msg->data[4];
    *dminutes = msg->data[5] << 8 | msg->data[6];
    *direction = msg->data[7];

    return true;
}

bool get_gps_lon(
    const can_msg_t *msg, uint8_t *degrees, uint8_t *minutes, uint16_t *dminutes, uint8_t *direction
) {
    if (!msg) {
        return false;
    }
    if (!degrees) {
        return false;
    }
    if (!minutes) {
        return false;
    }
    if (!dminutes) {
        return false;
    }
    if (!direction) {
        return false;
    }
    if (get_message_type(msg) != MSG_GPS_LONGITUDE) {
        return false;
    }

    *degrees = msg->data[3];
    *minutes = msg->data[4];
    *dminutes = msg->data[5] << 8 | msg->data[6];
    *direction = msg->data[7];

    return true;
}

bool get_gps_alt(const can_msg_t *msg, uint16_t *altitude, uint8_t *daltitude, uint8_t *units) {
    if (!msg) {
        return false;
    }
    if (!altitude) {
        return false;
    }
    if (!daltitude) {
        return false;
    }
    if (!units) {
        return false;
    }
    if (get_message_type(msg) != MSG_GPS_ALTITUDE) {
        return false;
    }

    *altitude = (uint16_t)msg->data[3] << 8 | msg->data[4];
    *daltitude = msg->data[5];
    *units = msg->data[6];

    return true;
}

bool get_gps_info(const can_msg_t *msg, uint8_t *num_sat, uint8_t *quality) {
    if (!msg) {
        return false;
    }
    if (!num_sat) {
        return false;
    }
    if (!quality) {
        return false;
    }
    if (get_message_type(msg) != MSG_GPS_INFO) {
        return false;
    }

    *num_sat = msg->data[3];
    *quality = msg->data[4];

    return true;
}
