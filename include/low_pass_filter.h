#ifndef ROCKETLIB_LOW_PASS_FILTER_H
#define ROCKETLIB_LOW_PASS_FILTER_H

#include "common.h"
#include <xc.h>

#define SAMPLE_FREQ(time_diff_ms) (1000.0 / (time_diff_ms))
#define LOW_PASS_ALPHA(TR) ((SAMPLE_FREQ * TR / 5.0) / (1 + SAMPLE_FREQ * TR / 5.0))

status_t low_pass_filter_init(double response_time);
uint16_t update_low_pass(uint16_t new_value, double *low_pass_value);

#endif /* ROCKETLIB_LOW_PASS_FILTER_H */
