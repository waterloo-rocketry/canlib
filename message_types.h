#ifndef MESSAGE_TYPES_H_
#define MESSAGE_TYPES_H_

/* The defines in this file track the information in this spreadsheet:
 * https://docs.google.com/spreadsheets/d/1sXmhBklBM_79vqq_NNaoq-xuxz7BbkHtFnsmBFPSksc/
 *
 * Message SID's are 11 bit unique identifiers. The bottom 5 bits of
 * the SID are ID's that are unique to the board sending the message,
 * and the top 6 bits are the message type. These defines all have the
 * bottom 5 bits set to 0, so to create a complete message SID,
 * bitwise or one of these defines with a board unique ID
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

#endif // compile guard
