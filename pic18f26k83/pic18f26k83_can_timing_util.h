#ifndef ROCKETLIB_TIMING_UTIL_H
#define ROCKETLIB_TIMING_UTIL_H

#include <stdbool.h>
#include <stdint.h>

#include "common/common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CAN_BIT_TIME_US 4

// Timing parameters
typedef struct {
	// BaudRate Prescaler
	uint8_t brp;
	// Synchronization Jump Width
	uint8_t sjw;

	// sample once or three times
	uint8_t sam;
	// phase segment 1 bits
	uint8_t seg1ph;
	// phase segment 2 bits
	uint8_t seg2ph;
	// propagation time segment bits
	uint8_t prseg;

	// Phase segment 2 time select bit. If true, then use seg2ph,
	// otherwise take minimum viable phase length
	bool btlmode;
} can_timing_t;

/**
 * @brief Generate CAN bus timing setting for PIC microcontrollers
 * @param system_freq PIC Fosc frequency in Hz
 * @param timing buffer to write timing parameters to
 * @return `W_SUCCESS` if system_freq is valid and parameters have been written to `timing`
 */
w_status_t pic18f26k83_can_generate_timing_params(uint32_t system_freq, can_timing_t *timing);

#ifdef __cplusplus
}
#endif

#endif
