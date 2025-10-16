#ifndef CANLIB_COMMON_H
#define CANLIB_COMMON_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

// these symbol should be defined in the project's Makefile, but if it isn't, issue a warning and
// set it to 0
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

#ifdef __cplusplus
extern "C" {
#endif

static inline void write_timestamp_2bytes(uint16_t timestamp, can_msg_t *output) {
	output->data[0] = (timestamp >> 8) & 0xff;
	output->data[1] = (timestamp >> 0) & 0xff;
}

static inline can_msg_type_t get_message_type(const can_msg_t *msg) {
	if (!msg) {
		return (can_msg_type_t)0;
	}

	return (can_msg_type_t)((msg->sid >> 18) & 0x1FF);
}

static inline uint8_t get_board_type_unique_id(const can_msg_t *msg) {
	if (!msg) {
		return 0;
	}

	return (uint8_t)((msg->sid >> 8) & 0xFF);
}

static inline uint8_t get_board_inst_unique_id(const can_msg_t *msg) {
	if (!msg) {
		return 0;
	}

	return (uint8_t)(msg->sid & 0xFF);
}

static inline uint16_t get_timestamp(const can_msg_t *msg) {
	if (!msg) {
		return 0;
	}

	return ((uint16_t)msg->data[0] << 8) | msg->data[1];
}

#ifdef __cplusplus
}
#endif

#endif
