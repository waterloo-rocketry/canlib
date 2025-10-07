#ifndef CANLIB_GPS_H
#define CANLIB_GPS_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Used to format GPS timestamp data. Data arguments: UTC time in hours,
 * minutes, seconds, and deci-seconds.
 */
bool build_gps_time_msg(can_msg_prio_t prio, uint16_t timestamp,
                        uint8_t utc_hours, uint8_t utc_mins, uint8_t utc_secs,
                        uint8_t utc_dsecs, can_msg_t *output);

/*
 * Used to send GPS latitude data. The format is degrees, minutes, and
 * direction. The minute value is split into integral and decimal parts.
 * Direction is either 'N' or 'S'
 */
bool build_gps_lat_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t degrees,
                       uint8_t minutes, uint16_t dminutes, uint8_t direction,
                       can_msg_t *output);

/*
 * Used to send GPS longitude data. The format is degrees, minutes, and
 * direction. The minute value is split into integral and decimal parts.
 * Direction is either 'E' or 'W'
 */
bool build_gps_lon_msg(can_msg_prio_t prio, uint16_t timestamp, uint8_t degrees,
                       uint8_t minutes, uint16_t dminutes, uint8_t direction,
                       can_msg_t *output);

/*
 * Used to send GPS altitude data. Altitude is broken into integral
 * and decimal parts. Units are either 'M' (metres) or 'F' (feet).
 */
bool build_gps_alt_msg(can_msg_prio_t prio, uint16_t timestamp,
                       uint16_t altitude, uint8_t daltitude, uint8_t units,
                       can_msg_t *output);

/*
 * Used to send general GPS info. Currently sends the number of
 * satellites used to obtain a reading and the quality indicator.
 */
bool build_gps_info_msg(can_msg_prio_t prio, uint16_t timestamp,
                        uint8_t num_sat, uint8_t quality, can_msg_t *output);

/*
 * Gets GPS UTC time information. Format is UTC hours, minutes,
 * seconds, and deci-seconds.
 */
bool get_gps_time(const can_msg_t *msg, uint8_t *utc_hours, uint8_t *utc_mins,
                  uint8_t *utc_secs, uint8_t *utc_dsecs);

/*
 * Gets GPS latitude information. Format is degrees, minutes,
 * direction. The minutes value is broken into integral and decimal
 * parts. The direction is either 'N' or 'S'.
 */
bool get_gps_lat(const can_msg_t *msg, uint8_t *degrees, uint8_t *minutes,
                 uint16_t *dminutes, uint8_t *direction);

/*
 * Gets GPS longitude information. Format is degrees, minutes,
 * direction. The minutes value is broken into integral and decimal
 * parts. The direction is either 'E' or 'W'.
 */
bool get_gps_lon(const can_msg_t *msg, uint8_t *degrees, uint8_t *minutes,
                 uint16_t *dminutes, uint8_t *direction);

/*
 * Gets GPS altitude information. The altitude reading is broken
 * into integral and decimal parts. The units are either 'M' (meters)
 * or 'F' (feet).
 */
bool get_gps_alt(const can_msg_t *msg, uint16_t *altitude, uint8_t *daltitude,
                 uint8_t *units);

/*
 * Gets GPS general info: number of satellites used in a reading and
 * quality indicator.
 */
bool get_gps_info(const can_msg_t *msg, uint8_t *num_sat, uint8_t *quality);

#ifdef __cplusplus
}
#endif

#endif
