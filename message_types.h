#ifndef MESSAGE_TYPES_H_
#define MESSAGE_TYPES_H_

/* REMINDER: Any changes to this file should be reflected in parsley.
 *
 * If parsley and this file differ, this file is the source of truth.
 *
 * Message SIDs are 11 bit unique identifiers.
 * Bottom 5 bits: IDs that are unique to the board sending the message
 * Top 6 bits: message types. The message types defined here therefore
 * have the bottom 5 bits set to 0.
 *
 * MESSAGE_SID = MESSAGE_TYPE | BOARD_UNIQUE_ID
 *
 */

// Message Types
#define MSG_GENERAL_CMD           0x060
#define MSG_ACTUATOR_CMD          0x0C0
#define MSG_ALT_ARM_CMD           0x140
#define MSG_RESET_CMD             0x160

#define MSG_DEBUG_MSG             0x180
#define MSG_DEBUG_PRINTF          0x1E0
#define MSG_DEBUG_RADIO_CMD       0x200
#define MSG_ACT_ANALOG_CMD        0x220


#define MSG_ALT_ARM_STATUS        0x440
#define MSG_ACTUATOR_STATUS       0x460
#define MSG_GENERAL_BOARD_STATUS  0x520

#define MSG_SENSOR_TEMP           0x540
#define MSG_SENSOR_ALTITUDE       0x560
#define MSG_SENSOR_ACC            0x580
#define MSG_SENSOR_ACC2           0x5A0
#define MSG_SENSOR_GYRO           0x5E0

#define MSG_STATE_EST_CALIB       0x620
#define MSG_SENSOR_MAG            0x640
#define MSG_SENSOR_ANALOG         0x6A0
#define MSG_GPS_TIMESTAMP         0x6C0
#define MSG_GPS_LATITUDE          0x6E0
#define MSG_GPS_LONGITUDE         0x700
#define MSG_GPS_ALTITUDE          0x720
#define MSG_GPS_INFO              0x740


#define MSG_FILL_LVL              0x780
#define MSG_STATE_EST_DATA        0x7A0


#define MSG_LEDS_ON               0x7E0
#define MSG_LEDS_OFF              0x7C0

// Board IDs
// Any
#define BOARD_ID_ANY               0x00
// Ground Side
#define BOARD_ID_DAQ               0x01
#define BOARD_ID_THERMOCOUPLE_1    0x02
#define BOARD_ID_THERMOCOUPLE_2    0x03
#define BOARD_ID_THERMOCOUPLE_3    0x04
#define BOARD_ID_THERMOCOUPLE_4    0x05
// Injector/Fill Section
#define BOARD_ID_PROPULSION_INJ    0x06
// Vent Section
#define BOARD_ID_PROPULSION_VENT   0x07
#define BOARD_ID_CAMERA_1          0x08
#define BOARD_ID_CAMERA_2          0x09
// Airbrake Section
#define BOARD_ID_CHARGING_AIRBRAKE 0x0A
// Payload Section
#define BOARD_ID_CHARGING_PAYLOAD  0x0B
#define BOARD_ID_VIBRATION         0x0C
// Recovery Electronics(RecElec) Sled
#define BOARD_ID_CHARGING_CAN      0x0D
#define BOARD_ID_LOGGER            0x0E
#define BOARD_ID_PROCESSOR         0x0F
#define BOARD_ID_GPS               0x10
#define BOARD_ID_ARMING            0x11
#define BOARD_ID_TELEMETRY         0x12
#define BOARD_ID_CAMERA_3          0x13
// Debug
#define BOARD_ID_USB               0x14

/*
 * General message type format (from spreadsheet):
 * (Version 0.7.0)
 *                  byte 0      byte 1      byte 2      byte 3                byte 4           byte 5             byte 6           byte 7
 * GENERAL_CMD:     TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L COMMAND_TYPE          None             None               None             None
 * ACTUATOR_CMD:    TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L ACTUATOR_ID           ACTUATOR_STATE   None               None             None
 * ALT_ARM_CMD:     TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L ALT_ARM_STATE & #     None             None               None             None
 * RESET_CMD:       TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L BOARD_ID              None             None               None             None
 * ACT_ANALOG_CMD:  TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L ACTUATOR_ID           ACT_STATE_INT    None               None             None
 *
 * DEBUG_MSG:       TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L DEBUG_LEVEL | LINUM_H LINUM_L          MESSAGE_DEFINED    MESSAGE_DEFINED  MESSAGE_DEFINED
 * DEBUG_PRINTF:    ASCII       ASCII       ASCII       ASCII                 ASCII            ASCII              ASCII            ASCII
 * DEBUG_RADIO_CMD: ASCII       ASCII       ASCII       ASCII                 ASCII            ASCII              ASCII            ASCII
 *
 * ACTUATOR_STAT:   TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L ACTUATOR_ID           ACTUATOR_STATE   REQ_ACTUATOR_STATE None             None
 * ALT_ARM_STAT:    TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L ALT_ARM_STATE & #     V_DROGUE_H       V_DROGUE_L         V_MAIN_H         V_MAIN_L
 * BOARD_STAT:      TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L ERROR_CODE            BOARD_DEFINED    BOARD_DEFINED      BOARD_DEFINED    BOARD_DEFINED
 *
 * SENSOR_TEMP:     TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L SENSOR_NUM            TEMP_H           TEMP_M             TEMP_L           None
 * SENSOR_ALTITUDE: TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L ALTITUDE_H            ALTITUDE_MH      ALTITUDE_ML        ALTITUDE_L       None
 * SENSOR_ACC:      TSTAMP_MS_M TSTAMP_MS_L VALUE_X_H   VALUE_X_L             VALUE_Y_H        VALUE_Y_L          VALUE_Z_H        VALUE_Z_L
 * SENSOR_GYRO:     TSTAMP_MS_M TSTAMP_MS_L VALUE_X_H   VALUE_X_L             VALUE_Y_H        VALUE_Y_L          VALUE_Z_H        VALUE_Z_L
 * SENSOR_MAG:      TSTAMP_MS_M TSTAMP_MS_L VALUE_X_H   VALUE_X_L             VALUE_Y_H        VALUE_Y_L          VALUE_Z_H        VALUE_Z_L
 * SENSOR_ANALOG:   TSTAMP_MS_M TSTAMP_MS_L SENSOR_ID   VALUE_H               VALUE_L          None               None             None
 *
 * GPS_TIMESTAMP:   TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L UTC_HOURS             UTC_MINUTES      UTC_SECONDS        UTC_DSECONDS     None
 * GPS_LAT:         TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L DEGREES               MINUTES          DMINUTES_H         DIMNUTES_L       N/S DIRECTION
 * GPS_LON:         TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L DEGREES               MINUTES          DMINUTES_H         DIMNUTES_L       E/W DIRECTION
 * GPS_ALT:         TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L ALT_H                 ALT_L            ALT_DEC            UNITS            None
 * GPS_INFO:        TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L NUM_SAT               QUALITY          None               None             None
 * 
 *
 * FILL_LVL:        TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L FILL_LEVEL            DIRECTION        None               None             None
 *
 * STATE_EST_DATA:  TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L DATA_L                DATA_ML          DATA_MH            DATA_H           STATE_ID
 * STATE_EST_CALIB: TSTAMP_MS_H TSTAMP_MS_M TSTAMP_MS_L ACK_FLAG              APOGEE_H         APOGEE_L           None             None
 *
 * LEDS_ON:         None        None        None        None                  None             None               None             None
 * LEDS_OFF:        None        None        None        None                  None             None               None             None
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

// ACTUATOR_CMD/STATUS STATES
enum ACTUATOR_STATE {
    ACTUATOR_ON = 0,
    ACTUATOR_OFF,
    ACTUATOR_UNK,
    ACTUATOR_ILLEGAL,
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

    E_5V_OVER_CURRENT,         // mA_high          mA_low              x                   x
    E_5V_UNDER_VOLTAGE,        // mV_high          mV_low              x                   x
    E_5V_OVER_VOLTAGE,         // mV_high          mV_low              x                   x

    E_BATT_OVER_CURRENT,       // mA_high          mA_low              x                   x
    E_BATT_UNDER_VOLTAGE,      // mV_high          mV_low              x                   x
    E_BATT_OVER_VOLTAGE,       // mV_high          mV_low              x                   x

    E_13V_OVER_CURRENT,        // mA_high          mA_low              x                   x
    E_MOTOR_OVER_CURRENT,      // mA_high          mA_low              x                   x

    E_BOARD_FEARED_DEAD,        // board_id         x                   x                   x
    E_NO_CAN_TRAFFIC,           // time_s_high      time_s_low          x                   x
    E_MISSING_CRITICAL_BOARD,   // board_id         x                   x                   x
    E_RADIO_SIGNAL_LOST,        // time_s_high      time_s_low          x                   x

    E_ACTUATOR_STATE,           // expected_state   actuator_state      x                   x
    E_CANNOT_INIT_DACS,         // x                x                   x                   x
    E_VENT_POT_RANGE,           // lim_upper (mV)   lim_lower (mV)      pot (mV)            x

    E_LOGGING,                  // error_type       x                   x                   x
    E_GPS,                      // x                x                   x                   x
    E_SENSOR,                   // sensor_id        x                   x                   x
    E_VIDEO,                    // state            x                   x                   x

    E_ILLEGAL_CAN_MSG,          // x                x                   x                   x
    E_SEGFAULT,                 // x                x                   x                   x
    E_UNHANDLED_INTERRUPT,      // x                x                   x                   x
    E_CODING_SCREWUP,           // x                x                   x                   x
};

enum SENSOR_ID {
    SENSOR_5V_CURR = 0,
    SENSOR_BATT_CURR,
    SENSOR_BATT_VOLT,
    SENSOR_CHARGE_CURR,
    SENSOR_13V_CURR,
    SENSOR_MOTOR_CURR,
    SENSOR_GROUND_VOLT,
    SENSOR_PRESSURE_OX,
    SENSOR_PRESSURE_FUEL,
    SENSOR_PRESSURE_CC,
    SENSOR_PRESSURE_PNEUMATICS,
    SENSOR_HALL_OX_INJ,
    SENSOR_HALL_FUEL_INJ,
    SENSOR_HALL_FILL,
    SENSOR_BARO,
    SENSOR_ARM_BATT_1,
    SENSOR_ARM_BATT_2,
    SENSOR_MAG_1,
    SENSOR_MAG_2,
    SENSOR_VELOCITY,
    SENSOR_VENT_TEMP,
    SENSOR_RADIO_CURR,
    SENSOR_PAYLOAD_TEMP,
    SENSOR_PAYLOAD_FLOW_RATE,
    SENSOR_9V_BATT_CURR_1,
    SENSOR_9V_BATT_CURR_2,
    SENSOR_FPS
};

enum FILL_DIRECTION {
    FILLING = 0,
    EMPTYING,
};

enum ACTUATOR_ID {
    ACTUATOR_VENT_VALVE = 0,
    ACTUATOR_INJECTOR_VALVE,
    ACTUATOR_FILL_DUMP_VALVE,
    ACTUATOR_CAMERA_1,
    ACTUATOR_CAMERA_2,
    ACTUATOR_CANBUS,
    ACTUATOR_CHARGE_CAN,
    ACTUATOR_RADIO,
    ACTUATOR_PAYLOAD_SERVO,
    ACTUATOR_AIRBRAKES_SERVO,
    ACTUATOR_AIRBRAKES_ENABLE,
    ACTUATOR_ROCKET_POWER,
    ACTUATOR_OX_INJECTOR, // Used for hall-sensor state feedback only
    ACTUATOR_FUEL_INJECTOR, // Used for hall-sensor state feedback only
    ACTUATOR_CHARGE_AIRBRAKE,
    ACTUATOR_CHARGE_PAYLOAD
};

enum STATE_ID {
    STATE_POS_X = 0,
    STATE_POS_Y,
    STATE_POS_Z,
    STATE_VEL_X,
    STATE_VEL_Y,
    STATE_VEL_Z,
    STATE_ACC_X,
    STATE_ACC_Y,
    STATE_ACC_Z,
    STATE_ANGLE_YAW,
    STATE_ANGLE_PITCH,
    STATE_ANGLE_ROLL,
    STATE_RATE_YAW,
    STATE_RATE_PITCH,
    STATE_RATE_ROLL,
    STATE_FILTER_YAW,
    STATE_FILTER_PITCH,
    STATE_FILTER_ROLL
};

#endif // compile guard
