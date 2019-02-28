#ifndef TIMING_UTIL_H_
#define TIMING_UTIL_H_

#include "can.h"
#include <stdint.h>
#include <stdbool.h>

bool can_generate_timing_params(uint32_t can_frequency, can_timing_t *timing);

#endif
