#ifndef MESSAGE_TYPES_H_
#define MESSAGE_TYPES_H_

/* The defines in this file track the information in this spreadsheet:
 * https://docs.google.com/spreadsheets/d/1sXmhBklBM_79vqq_NNaoq-xuxz7BbkHtFnsmBFPSksc/
 *
 * If the spreadsheet and file differ, this file is the source of truth.
 *
 * Message SIDs are 11 bit unique identifiers.
 * Bottom 5 bits: IDs that are unique to the board sending the message
 * Top 6 bits: message types. The message types defined here therefore
 * have the bottom 5 bits set to 0.
 *
 * MESSAGE_SID = MESSAGE_TYPE | BOARD_UNIQUE_ID
 *
 */

#define MSG_GENERAL_CMD           0x060
#define MSG_VENT_VALVE_CMD        0x0C0
#define MSG_INJ_VALVE_CMD         0x120
#define MSG_ALT_ARM_CMD           0x140
#define MSG_RESET_CMD             0x160

#define MSG_DEBUG_MSG             0x180
#define MSG_DEBUG_PRINTF          0x1E0
#define MSG_DEBUG_RADIO_CMD       0x200

#define MSG_ALT_ARM_STATUS        0x440
#define MSG_VENT_VALVE_STATUS     0x460
#define MSG_INJ_VALVE_STATUS      0x4C0
#define MSG_GENERAL_BOARD_STATUS  0x520

#define MSG_SENSOR_ALTITUDE       0x560
#define MSG_SENSOR_ACC            0x580
#define MSG_SENSOR_GYRO           0x5E0
#define MSG_SENSOR_MAG            0x640
#define MSG_SENSOR_ANALOG         0x6A0

#define MSG_GPS_TIMESTAMP         0x6C0
#define MSG_GPS_LATITUDE          0x6E0
#define MSG_GPS_LONGITUDE         0x700
#define MSG_GPS_ALTITUDE          0x720
#define MSG_GPS_INFO              0x740

#define MSG_FILL_LVL              0x780

#define MSG_RADI_VALUE            0x7A0

#define MSG_LEDS_ON               0x7E0
#define MSG_LEDS_OFF              0x7C0


#define BOARD_ID_INJECTOR         0x01
#define BOARD_ID_INJECTOR_SPARE   0x02
#define BOARD_ID_LOGGER           0x03
#define BOARD_ID_LOGGER_SPARE     0x04
#define BOARD_ID_RADIO            0x05
#define BOARD_ID_RADIO_SPARE      0x06
#define BOARD_ID_SENSOR           0x07
#define BOARD_ID_SENSOR_SPARE     0x08
#define BOARD_ID_USB              0x09
#define BOARD_ID_USB_SPARE        0x0A
#define BOARD_ID_VENT             0x0B
#define BOARD_ID_VENT_SPARE       0x0C
#define BOARD_ID_GPS              0x0D
#define BOARD_ID_GPS_SPARE        0x0E
#define BOARD_ID_FILL             0x0F
#define BOARD_ID_FILL_SPARE       0x10
#define BOARD_ID_ARMING           0x11
#define BOARD_ID_ARMING_SPARE     0x12
#define BOARD_ID_PAPA             0x13
#define BOARD_ID_PAPA_SPARE       0x14

/*
 * General message type format (from spreadsheet):
 * (Version 0.6.0)
 *                  byte 0      byte 1       byte 2         byte 3                  byte 4          byte 5          byte 6          byte 7
 * GENERAL_CMD:     TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    COMMAND_TYPE            None            None            None            None
 * VENT_VALVE_CMD:  TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    VENT_VALVE_STATE        None            None            None            None
 * INJ_VALVE_CMD:   TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    INJ_VALVE_STATE         None            None            None            None
 * ALT_ARM_CMD:     TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    ALT_ARM_STATE & #       None            None            None            None
 * RESET_CMD:       TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    BOARD_ID                None            None            None            None
 *
 * DEBUG_MSG:       TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    DEBUG_LEVEL | LINUM_H   LINUM_L         MESSAGE_DEFINED MESSAGE_DEFINED MESSAGE_DEFINED
 * DEBUG_PRINTF:    ASCII       ASCII        ASCII          ASCII                   ASCII           ASCII           ASCII           ASCII
 * DEBUG_RADIO_CMD: ASCII       ASCII        ASCII          ASCII                   ASCII           ASCII           ASCII           ASCII
 *
 * VENT_VALVE_STAT: TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    VENT_VALVE_STATE        CMD_VALVE_STATE None            None            None
 * INJ_VALVE_STAT:  TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    INJ_VALVE_STATE         CMD_VALVE_STATE None            None            None
 * ALT_ARM_STAT:    TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    ALT_ARM_STATE & #       V_DROGUE_H      V_DROGUE_L      V_MAIN_H        V_MAIN_L
 * BOARD_STAT:      TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    ERROR_CODE              BOARD_DEFINED   BOARD_DEFINED   BOARD_DEFINED   BOARD_DEFINED
 *
 * SENSOR_ALTITUDE: TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    ALTITUDE_H              ALTITUDE_MH     ALTITUDE_ML     ALTITUDE_L      None
 * SENSOR_ACC:      TSTAMP_MS_M TSTAMP_MS_L  VALUE_X_H      VALUE_X_L               VALUE_Y_H       VALUE_Y_L       VALUE_Z_H       VALUE_Z_L
 * SENSOR_GYRO:     TSTAMP_MS_M TSTAMP_MS_L  VALUE_X_H      VALUE_X_L               VALUE_Y_H       VALUE_Y_L       VALUE_Z_H       VALUE_Z_L
 * SENSOR_MAG:      TSTAMP_MS_M TSTAMP_MS_L  VALUE_X_H      VALUE_X_L               VALUE_Y_H       VALUE_Y_L       VALUE_Z_H       VALUE_Z_L
 * SENSOR_ANALOG:   TSTAMP_MS_M TSTAMP_MS_L  SENSOR_ID      VALUE_H                 VALUE_L         None            None            None
 *
 * GPS_TIMESTAMP:   TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    UTC_HOURS               UTC_MINUTES     UTC_SECONDS     UTC_DSECONDS    None
 * GPS_LAT:         TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    DEGREES                 MINUTES         DMINUTES_H      DIMNUTES_L      N/S DIRECTION
 * GPS_LON:         TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    DEGREES                 MINUTES         DMINUTES_H      DIMNUTES_L      E/W DIRECTION
 * GPS_ALT:         TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    ALT_H                   ALT_L           ALT_DEC         UNITS           None
 * GPS_INFO:        TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    NUM_SAT                 QUALITY         None            None            None
 *
 * FILL_LVL:        TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    FILL_LEVEL              DIRECTION       None            None            None
 * 
 * RADI_VALUE:      TSTAMP_MS_H TSTAMP_MS_M  TSTAMP_MS_L    RADI_BOARD              RADI_HIGH       RADI_LOW        None            None
 * 
 * LEDS_ON:         None        None         None           None                    None            None            None            None
 * LEDS_OFF:        None        None         None           None                    None            None            None            None
 *
 * This file defines the format of the various CAN message types (defined in
 * message_types.h). There is no unified message format; the format of each message
 * depends on the message type. In the case of board status messages, the format
 * of the rest of the message depends on the message's error code.
 *
 * This file is the source of truth for message formats. Refer to this when parsing
 * CAN messages. Increment the version number when the format for a new message
 * type is added.
 */

// GENERAL_CMD: COMMAND_TYPE
enum GEN_CMD {
    BUS_DOWN_WARNING = 0,
};

// VALVE_CMD/STATUS STATES
enum VALVE_STATE {
    VALVE_OPEN = 0,
    VALVE_CLOSED,
    VALVE_UNK,
    VALVE_ILLEGAL,
};

// ARM_CMD/STATUS STATES
enum ARM_STATE {
    DISARMED = 0,
    ARMED,
};

// BOARD GENERAL STATUS ERROR CODES
//  ERROR CODE (byte 3)         (byte4)             (byte 5)            (byte 6)            (byte 7)
enum BOARD_STATUS {
    E_NOMINAL = 0,              // x                x                   x                   x

    E_BUS_OVER_CURRENT,         // mA_high          mA_low              x                   x
    E_BUS_UNDER_VOLTAGE,        // mV_high          mV_low              x                   x
    E_BUS_OVER_VOLTAGE,         // mV_high          mV_low              x                   x

    E_BATT_UNDER_VOLTAGE,       // mV_high          mV_low              x                   x
    E_BATT_OVER_VOLTAGE,        // mV_high          mV_low              x                   x

    E_BOARD_FEARED_DEAD,        // board_id         x                   x                   x
    E_NO_CAN_TRAFFIC,           // time_s_high      time_s_low          x                   x
    E_MISSING_CRITICAL_BOARD,   // board_id         x                   x                   x
    E_RADIO_SIGNAL_LOST,        // time_s_high      time_s_low          x                   x

    E_VALVE_STATE,              // expected_state   valve_state         x                   x
    E_CANNOT_INIT_DACS,         // x                x                   x                   x
    E_VENT_POT_RANGE,           // lim_upper (mV)   lim_lower (mV)      pot (mV)            x

    E_LOGGING,                  // error_type       x                   x                   x
    E_GPS,                      // x                x                   x                   x
    E_SENSOR,                   // sensor_id        x                   x                   x

    E_ILLEGAL_CAN_MSG,          // x                x                   x                   x
    E_SEGFAULT,                 // x                x                   x                   x
    E_UNHANDLED_INTERRUPT,      // x                x                   x                   x
    E_CODING_SCREWUP,           // x                x                   x                   x

    E_BATT_OVER_CURRENT,        // mA_high          mA_low              x                   x
};

enum SENSOR_ID {
    SENSOR_IMU1 = 0,
    SENSOR_IMU2,
    SENSOR_BARO,
    SENSOR_PRESSURE_OX,
    SENSOR_PRESSURE_CC,
    SENSOR_VENT_BATT,
    SENSOR_INJ_BATT,
    SENSOR_ARM_BATT_1,
    SENSOR_ARM_BATT_2,
    SENSOR_BATT_CURR,
    SENSOR_BUS_CURR,
    SENSOR_VELOCITY,
    SENSOR_MAG_1,
    SENSOR_MAG_2,
};

enum FILL_DIRECTION {
    FILLING = 0,
    EMPTYING,
};


#endif // compile guard
