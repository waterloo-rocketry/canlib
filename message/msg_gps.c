#include <stdbool.h>
#include <stdint.h>

#include "common.h"

#include "can.h"
#include "message_types.h"
#include "msg_common.h"
#include "msg_gps.h"

void build_gps_time_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t utc_hours,
						uint8_t utc_mins, uint8_t utc_secs, uint8_t utc_dsecs, can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_GPS_TIMESTAMP, 0);
	write_timestamp(timestamp, output);

	output->data[2] = utc_hours;
	output->data[3] = utc_mins;
	output->data[4] = utc_secs;
	output->data[5] = utc_dsecs;

	output->data_len = 6;
}

void build_gps_lat_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t degrees, uint8_t minutes,
					   uint16_t dminutes, uint8_t direction, can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_GPS_LATITUDE, 0);
	write_timestamp(timestamp, output);

	output->data[2] = degrees;
	output->data[3] = minutes;
	output->data[4] = dminutes >> 8;
	output->data[5] = dminutes & 0xFF;
	output->data[6] = direction;

	output->data_len = 7;
}

void build_gps_lon_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t degrees, uint8_t minutes,
					   uint16_t dminutes, uint8_t direction, can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_GPS_LONGITUDE, 0);
	write_timestamp(timestamp, output);

	output->data[2] = degrees;
	output->data[3] = minutes;
	output->data[4] = dminutes >> 8;
	output->data[5] = dminutes & 0xFF;
	output->data[6] = direction;

	output->data_len = 7;
}

void build_gps_alt_msg(can_msg_prio_t prio, uint16_t timestamp, uint16_t altitude,
					   uint8_t daltitude, uint8_t units, can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_GPS_ALTITUDE, 0);
	write_timestamp(timestamp, output);

	output->data[2] = (altitude >> 8) & 0xff;
	output->data[3] = (altitude >> 0) & 0xff;
	output->data[4] = daltitude;
	output->data[5] = units;

	output->data_len = 6;
}

void build_gps_info_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t num_sat, uint8_t quality,
						can_msg_t *output) {
	w_assert(output);

	output->sid = build_sid(prio, MSG_GPS_INFO, 0);
	write_timestamp(timestamp, output);

	output->data[2] = num_sat;
	output->data[3] = quality;

	output->data_len = 4;
}

bool get_gps_time(const can_msg_t *msg, uint8_t *utc_hours, uint8_t *utc_mins, uint8_t *utc_secs,
				  uint8_t *utc_dsecs) {
	w_assert(msg);
	w_assert(utc_hours);
	w_assert(utc_mins);
	w_assert(utc_secs);
	w_assert(utc_dsecs);

	if (get_message_type(msg) != MSG_GPS_TIMESTAMP) {
		return false;
	}

	*utc_hours = msg->data[2];
	*utc_mins = msg->data[3];
	*utc_secs = msg->data[4];
	*utc_dsecs = msg->data[5];

	return true;
}

bool get_gps_lat(const can_msg_t *msg, uint8_t *degrees, uint8_t *minutes, uint16_t *dminutes,
				 uint8_t *direction) {
	w_assert(msg);
	w_assert(degrees);
	w_assert(minutes);
	w_assert(dminutes);
	w_assert(direction);

	if (get_message_type(msg) != MSG_GPS_LATITUDE) {
		return false;
	}

	*degrees = msg->data[2];
	*minutes = msg->data[3];
	*dminutes = ((uint16_t)msg->data[4] << 8) | msg->data[5];
	*direction = msg->data[6];

	return true;
}

bool get_gps_lon(const can_msg_t *msg, uint8_t *degrees, uint8_t *minutes, uint16_t *dminutes,
				 uint8_t *direction) {
	w_assert(msg);
	w_assert(degrees);
	w_assert(minutes);
	w_assert(dminutes);
	w_assert(direction);

	if (get_message_type(msg) != MSG_GPS_LONGITUDE) {
		return false;
	}

	*degrees = msg->data[2];
	*minutes = msg->data[3];
	*dminutes = ((uint16_t)msg->data[4] << 8) | msg->data[5];
	*direction = msg->data[6];

	return true;
}

bool get_gps_alt(const can_msg_t *msg, uint16_t *altitude, uint8_t *daltitude, uint8_t *units) {
	w_assert(msg);
	w_assert(altitude);
	w_assert(daltitude);
	w_assert(units);

	if (get_message_type(msg) != MSG_GPS_ALTITUDE) {
		return false;
	}

	*altitude = ((uint16_t)msg->data[2] << 8) | msg->data[3];
	*daltitude = msg->data[4];
	*units = msg->data[5];

	return true;
}

bool get_gps_info(const can_msg_t *msg, uint8_t *num_sat, uint8_t *quality) {
	w_assert(msg);
	w_assert(num_sat);
	w_assert(quality);

	if (get_message_type(msg) != MSG_GPS_INFO) {
		return false;
	}

	*num_sat = msg->data[2];
	*quality = msg->data[3];

	return true;
}
