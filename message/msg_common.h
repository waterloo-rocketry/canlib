#ifndef CANLIB_COMMON_H
#define CANLIB_COMMON_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "message_types.h"

#ifndef LINT
#include "common.h"
#else
#define w_assert(statement)                                                                        \
	if (statement) {}
#endif

#ifndef CANLIB_DYNAMIC_BOARD_ID

#ifndef BOARD_TYPE_UNIQUE_ID
#warning BOARD_TYPE_UNIQUE_ID not defined, please set that up in project
#define BOARD_TYPE_UNIQUE_ID 0
#endif

#ifndef BOARD_INST_UNIQUE_ID
#warning BOARD_INST_UNIQUE_ID not defined, please set that up in project
#define BOARD_INST_UNIQUE_ID 0
#endif

static inline uint32_t build_sid(can_msg_prio_t prio, can_msg_type_t msg_type, uint8_t metadata) {
	w_assert(((uint32_t)prio & 0xfffffffc) == 0);
	w_assert(((uint32_t)msg_type & 0xffffff80) == 0);
	w_assert(((uint32_t)BOARD_TYPE_UNIQUE_ID & 0xffffffc0) == 0);
	w_assert(((uint32_t)BOARD_INST_UNIQUE_ID & 0xffffffc0) == 0);

	return (((uint32_t)prio) << 27) | (((uint32_t)msg_type) << 20) |
		   (((uint32_t)BOARD_TYPE_UNIQUE_ID) << 14) | (((uint32_t)BOARD_INST_UNIQUE_ID) << 8) |
		   metadata;
}

#else

extern uint8_t board_type_unique_id;
extern uint8_t board_inst_unique_id;

static inline uint32_t build_sid(can_msg_prio_t prio, can_msg_type_t msg_type, uint8_t metadata) {
	w_assert(((uint32_t)prio & 0xfffffffc) == 0);
	w_assert(((uint32_t)msg_type & 0xffffff80) == 0);
	w_assert(((uint32_t)board_type_unique_id & 0xffffffc0) == 0);
	w_assert(((uint32_t)board_inst_unique_id & 0xffffffc0) == 0);

	return (((uint32_t)prio) << 27) | (((uint32_t)msg_type) << 20) |
		   (((uint32_t)board_type_unique_id) << 14) | (((uint32_t)board_inst_unique_id) << 8) |
		   metadata;
}

#endif

// SID Handling Functions

static inline can_msg_type_t get_message_type(const can_msg_t *msg) {
	w_assert(msg);

	return (can_msg_type_t)((msg->sid >> 20) & 0x7f);
}

static inline uint8_t get_board_type_unique_id(const can_msg_t *msg) {
	w_assert(msg);

	return (msg->sid >> 14) & 0x3f;
}

static inline uint8_t get_board_inst_unique_id(const can_msg_t *msg) {
	w_assert(msg);

	return (msg->sid >> 8) & 0x3f;
}

static inline uint8_t get_message_metadata(const can_msg_t *msg) {
	w_assert(msg);

	return msg->sid & 0xff;
}

// Timestamp handle functions
static inline void write_timestamp(uint16_t timestamp, can_msg_t *output) {
	w_assert(output);

	output->data[0] = (timestamp >> 8) & 0xff;
	output->data[1] = (timestamp >> 0) & 0xff;
}

static inline uint16_t get_timestamp(const can_msg_t *msg) {
	w_assert(msg);

	return ((uint16_t)msg->data[0] << 8) | msg->data[1];
}

// Dynamic board ID specific function
#ifdef CANLIB_DYNAMIC_BOARD_ID

static inline void set_board_id(uint8_t board_type_id, uint8_t board_inst_id) {
	w_assert(((uint32_t)board_type_id & 0xc0) == 0);
	w_assert(((uint32_t)board_inst_id & 0xc0) == 0);

	board_type_unique_id = board_type_id;
	board_inst_unique_id = board_inst_id;
}

#endif

#endif
