#ifndef CANLIB_TIMING_UTIL_H
#define CANLIB_TIMING_UTIL_H

#include <stdbool.h>
#include <stdint.h>

#include "can.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate CAN bus timing setting for PIC microcontrollers
 * @param system_freq PIC Fosc frequency in Hz
 * @param timing buffer to write timing parameters to
 * @return `true` if system_freq is valid and parameters have been written to `timing`
 */
bool can_generate_timing_params(uint32_t system_freq, can_timing_t *timing);

#ifdef __cplusplus
}
#endif

#endif
