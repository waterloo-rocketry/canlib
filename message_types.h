#ifndef MESSAGE_TYPES_H_
#define MESSAGE_TYPES_H_

/* The defines in this file track the information in this spreadsheet:
 * https://docs.google.com/spreadsheets/d/1sXmhBklBM_79vqq_NNaoq-xuxz7BbkHtFnsmBFPSksc/
 *
 * Message SIDs are 11 bit unique identifiers.
 * Bottom 5 bits: IDs that are unique to the board sending the message
 * Top 6 bits: message types. The message types defined here therefore 
 * have the bottom 5 bits set to 0.
 *
 * MESSAGE_SID = MESSAGE_TYPE | BOARD_UNIQUE_ID
 *
 * Message type format (from spreadsheet):
 * (Version 0.0.3)
 *
 * GENERAL CMD:     TSTAMP_MS_H	TSTAMP_MS_M	 TSTAMP_MS_L    COMMAND_TYPE	        None	        None	        None	        None
 * VENT_VALVE_CMD:  TSTAMP_MS_H	TSTAMP_MS_M	 TSTAMP_MS_L	VENT_VALVE_STATE	    None	        None	        None	        None
 * INJ_VALVE_CMD:   TSTAMP_MS_H	TSTAMP_MS_M	 TSTAMP_MS_L	INJ_VALVE_STATE	        None	        None	        None	        None
 * DEBUG_MSG:       TSTAMP_MS_H	TSTAMP_MS_M	 TSTAMP_MS_L	DEBUG_LEVEL | LINUM_H	LINUM_L	        MESSAGE_DEFINED	MESSAGE_DEFINED	MESSAGE_DEFINED
 * DEBUG_PRINTF:    ASCII	    ASCII	     ASCII	        ASCII	                ASCII	        ASCII	        ASCII	        ASCII
 * VENT_VALVE_STAT: TSTAMP_MS_H	TSTAMP_MS_M	 TSTAMP_MS_L	VENT_VALVE_STATE	    None	        None	        None	        None
 * INJ_VALVE_STAT:  TSTAMP_MS_H	TSTAMP_MS_M	 TSTAMP_MS_L	INJ_VALVE_STATE	        None	        None	        None	        None
 * BOARD_STAT:      TSTAMP_MS_H	TSTAMP_MS_M	 TSTAMP_MS_L	ERROR_CODE	            BOARD_DEFINED	BOARD_DEFINED	BOARD_DEFINED	BOARD_DEFINED
 * SENSOR_ACC:      TSTAMP_MS_M	TSTAMP_MS_L	 VALUE_X_H	    VALUE_X_L	            VALUE_Y_H	    VALUE_Y_L	    VALUE_Z_H	    VALUE_Z_L
 * SENSOR_GYRO:     TSTAMP_MS_M	TSTAMP_MS_L	 VALUE_X_H	    VALUE_X_L	            VALUE_Y_H	    VALUE_Y_L	    VALUE_Z_H	    VALUE_Z_L
 * SENSOR_MAG:      TSTAMP_MS_M	TSTAMP_MS_L	 VALUE_X_H	    VALUE_X_L	            VALUE_Y_H	    VALUE_Y_L	    VALUE_Z_H	    VALUE_Z_L
 * SENSOR_ANALOG:   TSTAMP_MS_M	TSTAMP_MS_L	 SENSOR_ID      VALUE_H	                VALUE_L	        None	        None	        None
 * LEDS_ON:         None	    None	     None	        None	                None	        None	        None	        None
 * LEDS_OFF:        None	    None	     None	        None	                None	        None	        None	        None
 */

#define MSG_GENERAL_CMD           0x060
#define MSG_VENT_VALVE_CMD        0x0C0
#define MSG_INJ_VALVE_CMD         0x120

#define MSG_DEBUG_MSG             0x180
#define MSG_DEBUG_PRINTF          0x1E0

#define MSG_VENT_VALVE_STATUS     0x460
#define MSG_INJ_VALVE_STATUS      0x4C0
#define MSG_GENERAL_BOARD_STATUS  0x520

#define MSG_SENSOR_ACC            0x580
#define MSG_SENSOR_GYRO           0x5E0
#define MSG_SENSOR_MAG            0x640
#define MSG_SENSOR_ANALOG         0x6A0

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

#endif // compile guard
