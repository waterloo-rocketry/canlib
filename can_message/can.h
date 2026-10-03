#ifndef ROCKETLIB_CAN_H
#define ROCKETLIB_CAN_H

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t can_sid_t;

typedef struct {
	// Standard Identifier - 29 bits long
	can_sid_t sid;
	// How many bytes are used in data
	uint8_t data_len;
	// the data you want to transmit
	uint8_t data[8];
} can_msg_t;

#endif
