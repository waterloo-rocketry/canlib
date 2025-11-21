#ifndef CANLIB_COMMON_H
#define CANLIB_COMMON_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

#ifndef CANLIB_DYNAMIC_BOARD_ID

#ifndef BOARD_TYPE_UNIQUE_ID
#warning BOARD_TYPE_UNIQUE_ID not defined, please set that up in project
#define BOARD_TYPE_UNIQUE_ID 0
#endif

#ifndef BOARD_INST_UNIQUE_ID
#warning BOARD_INST_UNIQUE_ID not defined, please set that up in project
#define BOARD_INST_UNIQUE_ID 0
#endif

#define SID(prio, msg)                                                                             \
	(((uint32_t)prio << 27) | ((uint32_t)msg << 18) | ((uint32_t)BOARD_TYPE_UNIQUE_ID << 8) |      \
	 BOARD_INST_UNIQUE_ID)

#else

extern uint8_t board_type_unique_id;
extern uint8_t board_inst_unique_id;

#define SID(prio, msg)                                                                             \
	(((uint32_t)prio << 27) | ((uint32_t)msg << 18) | ((uint32_t)board_type_unique_id << 8) |      \
	 board_inst_unique_id)

#endif

#ifdef __cplusplus
extern "C" {
#endif

void write_timestamp_2bytes(uint16_t timestamp, can_msg_t *output);

can_msg_type_t get_message_type(const can_msg_t *msg);

uint8_t get_board_type_unique_id(const can_msg_t *msg);
uint8_t get_board_inst_unique_id(const can_msg_t *msg);

uint16_t get_timestamp(const can_msg_t *msg);

#ifdef CANLIB_DYNAMIC_BOARD_ID

void set_board_id(uint8_t board_type_id, uint8_t board_inst_id);

#endif

#ifdef __cplusplus
}
#endif

#endif
