#ifndef TIMING_UTIL_H_
#define TIMING_UTIL_H_

#include <stdbool.h>
#include <stdint.h>

#include "can.h"

bool can_generate_timing_params(uint32_t can_frequency, can_timing_t *timing);

#endif
