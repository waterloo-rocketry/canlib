// Auto generated file, do not edit directly

#ifndef _CANLIB_MESSAGE_TYPES_H
#define _CANLIB_MESSAGE_TYPES_H

// Message Priority
typedef enum {
    PRIO_HIGHEST = 0x0,
    PRIO_HIGH = 0x1,
    PRIO_MEDIUM = 0x2,
    PRIO_LOW = 0x3,
} can_msg_prio_t;

// Message Types
typedef enum {
    MSG_GENERAL_BOARD_STATUS = 0x001,
    MSG_RESET_CMD = 0x002,
    MSG_DEBUG_RAW = 0x003,
    MSG_CONFIG_SET = 0x004,
    MSG_CONFIG_STATUS = 0x005,
    MSG_ACTUATOR_CMD = 0x006,
    MSG_ACTUATOR_ANALOG_CMD = 0x007,
    MSG_ACTUATOR_STATUS = 0x008,
    MSG_ALT_ARM_CMD = 0x009,
    MSG_ALT_ARM_STATUS = 0x00A,
    MSG_SENSOR_TEMP = 0x00B,
    MSG_SENSOR_ALTITUDE = 0x00C,
    MSG_SENSOR_IMU_X = 0x00D,
    MSG_SENSOR_IMU_Y = 0x00E,
    MSG_SENSOR_IMU_Z = 0x00F,
    MSG_SENSOR_MAG_X = 0x010,
    MSG_SENSOR_MAG_Y = 0x011,
    MSG_SENSOR_MAG_Z = 0x012,
    MSG_SENSOR_ANALOG = 0x013,
    MSG_GPS_TIMESTAMP = 0x014,
    MSG_GPS_LATITUDE = 0x015,
    MSG_GPS_LONGITUDE = 0x016,
    MSG_GPS_ALTITUDE = 0x017,
    MSG_GPS_INFO = 0x018,
    MSG_STATE_EST_DATA = 0x019,
    MSG_LEDS_ON = 0x01A,
    MSG_LEDS_OFF = 0x01B,
} can_msg_type_t;

// Board Type IDs
typedef enum {
    BOARD_TYPE_ID_ANY = 0x00,
    BOARD_TYPE_ID_INJ_SENSOR = 0x01,
    BOARD_TYPE_ID_CANARD_MOTOR = 0x02,
    BOARD_TYPE_ID_CAMERA = 0x03,
    BOARD_TYPE_ID_POWER = 0x04,
    BOARD_TYPE_ID_LOGGER = 0x05,
    BOARD_TYPE_ID_PROCESSOR = 0x06,
    BOARD_TYPE_ID_TELEMETRY = 0x07,
    BOARD_TYPE_ID_GPS = 0x08,
    BOARD_TYPE_ID_SRAD_GNSS = 0x09,
    BOARD_TYPE_ID_ALTIMETER = 0x0A,
    BOARD_TYPE_ID_ARMING = 0x0B,
    BOARD_TYPE_ID_PAY_SENSOR = 0x40,
    BOARD_TYPE_ID_PAY_MOTOR = 0x41,
    BOARD_TYPE_ID_RLCS_GLS = 0x80,
    BOARD_TYPE_ID_RLCS_RELAY = 0x81,
    BOARD_TYPE_ID_RLCS_HEATING = 0x82,
    BOARD_TYPE_ID_DAQ = 0x83,
    BOARD_TYPE_ID_CHARGING = 0x84,
    BOARD_TYPE_ID_THERMOCOUPLE = 0x85,
    BOARD_TYPE_ID_USB = 0x86,
    BOARD_TYPE_ID_FYDP25_TVCA = 0xC0,
} can_board_type_id_t;

// Board Instance IDs
typedef enum {
    BOARD_INST_ID_ANY = 0x00,
    BOARD_INST_ID_GENERIC = 0x01,
} can_board_inst_id_t;

typedef enum {
    BOARD_INST_ID_CAMERA_INJ_A = 0x02,
    BOARD_INST_ID_CAMERA_INJ_B = 0x03,
    BOARD_INST_ID_CAMERA_VENT_A = 0x04,
    BOARD_INST_ID_CAMERA_VENT_B = 0x05,
    BOARD_INST_ID_CAMERA_VENT_C = 0x06,
    BOARD_INST_ID_CAMERA_VENT_D = 0x07,
    BOARD_INST_ID_CAMERA_RECOVERY = 0x08,
} can_board_inst_id_camera_t;

typedef enum {
    BOARD_INST_ID_POWER_ROCKET = 0x09,
    BOARD_INST_ID_POWER_PAYLOAD = 0x0A,
} can_board_inst_id_power_t;

typedef enum {
    BOARD_INST_ID_THERMOCOUPLE_1 = 0x0B,
    BOARD_INST_ID_THERMOCOUPLE_2 = 0x0C,
    BOARD_INST_ID_THERMOCOUPLE_3 = 0x0D,
    BOARD_INST_ID_THERMOCOUPLE_4 = 0x0E,
} can_board_inst_id_thermocouple_t;

typedef enum {
    E_NOMINAL = 0x00,
    E_5V_OVER_CURRENT = 0x01,
    E_5V_OVER_VOLTAGE = 0x02,
    E_5V_UNDER_VOLTAGE = 0x04,
    E_12V_OVER_CURRENT = 0x08,
    E_12V_OVER_VOLTAGE = 0x10,
    E_12V_UNDER_VOLTAGE = 0x20,
    E_IO_ERROR = 0x40,
    E_FS_ERROR = 0x80,
} can_general_board_status_t;

typedef enum {
    ACTUATOR_OX_INJECTOR_VALVE = 0,
    ACTUATOR_FUEL_INJECTOR_VALVE,
    ACTUATOR_CHARGE_ENABLE,
    ACTUATOR_5V_RAIL_ROCKET,
    ACTUATOR_5V_RAIL_PAYLOAD,
    ACTUATOR_TELEMETRY,
    ACTUATOR_CAMERA_INJ_A,
    ACTUATOR_CAMERA_INJ_B,
    ACTUATOR_CAMERA_VENT_A,
    ACTUATOR_CAMERA_VENT_B,
    ACTUATOR_CAMERA_VENT_C,
    ACTUATOR_CAMERA_VENT_D,
    ACTUATOR_CAMERA_RECOVERY,
    ACTUATOR_PROC_ESTIMATOR_INIT,
    ACTUATOR_CANARD_ENABLE,
    ACTUATOR_CANARD_ANGLE,
} can_actuator_id_t;

typedef enum {
    ACTUATOR_ON = 0,
    ACTUATOR_OFF,
    ACTUATOR_UNK,
    ACTUATOR_ILLEGAL,
} can_actuator_state_t;

typedef enum {
    ALTIMETER_RAVEN = 0,
    ALTIMETER_STRATOLOGGER,
    ALTIMETER_SRAD,
} can_altimeter_id_t;

typedef enum {
    ALT_ARM_STATE_DISARMED = 0,
    ALT_ARM_STATE_ARMED,
} can_alt_arm_state_t;

typedef enum {
    IMU_PROC_POLULU_ALTIMU10 = 0,
    IMU_PROC_MOVELLA_MTI630,
    IMU_PROC_ST,
    IMU_SRAD_ALT_POLULU_ALTIMU10,
} can_imu_id_t;

typedef enum {
    SENSOR_5V_VOLT = 0,
    SENSOR_5V_CURR,
    SENSOR_12V_VOLT,
    SENSOR_12V_CURR,
    SENSOR_CHARGE_VOLT,
    SENSOR_CHARGE_CURR,
    SENSOR_BATT_VOLT,
    SENSOR_BATT_CURR,
    SENSOR_MOTOR_CURR,
    SENSOR_PRESSURE_OX,
    SENSOR_PRESSURE_FUEL,
    SENSOR_PRESSURE_CC,
    SENSOR_BARO_PRESSURE,
    SENSOR_BARO_TEMP,
    SENSOR_RA_BATT_VOLT_1,
    SENSOR_RA_BATT_VOLT_2,
    SENSOR_RA_BATT_CURR_1,
    SENSOR_RA_BATT_CURR_2,
    SENSOR_RA_MAG_VOLT_1,
    SENSOR_RA_MAG_VOLT_2,
    SENSOR_FPS,
    SENSOR_CANARD_ENCODER_1,
    SENSOR_CANARD_ENCODER_2,
    SENSOR_PROC_FLIGHT_PHASE_STATUS,
} can_analog_sensor_id_t;

typedef enum {
    STATE_ID_ATT_Q0 = 0,
    STATE_ID_ATT_Q1,
    STATE_ID_ATT_Q2,
    STATE_ID_ATT_Q3,
    STATE_ID_RATE_WX,
    STATE_ID_RATE_WY,
    STATE_ID_RATE_WZ,
    STATE_ID_VEL_VX,
    STATE_ID_VEL_VY,
    STATE_ID_VEL_VZ,
    STATE_ID_ALT,
    STATE_ID_COEFF_CL,
    STATE_ID_CANARD_ANGLE,
} can_state_est_id_t;

#endif
