#ifndef ROCKETLIB_LOW_PASS_FILTER_H
#define ROCKETLIB_LOW_PASS_FILTER_H

#include "common.h"
#include <stdbool.h>

// Macro to calculate the sample frequency based on the time difference in milliseconds
#define SAMPLE_FREQ(time_diff_ms) (1000.0 / (time_diff_ms))

// Macro to calculate the alpha value for the low-pass filter based on the response time
#define LOW_PASS_ALPHA(TR, time_diff_ms)                                                           \
    ((SAMPLE_FREQ(time_diff_ms) * TR / 5.0) / (1 + SAMPLE_FREQ(time_diff_ms) * TR / 5.0))

// Function prototype to initialize the low-pass filter
// Calculates and stores the alpha value based on the given response time
w_status_t low_pass_filter_init(double *alpha, double response_time);

// Function prototype to update the low-pass filter with a new value
// Updates the current filtered value and returns success status
bool update_low_pass(double *alpha, uint16_t new_value, double *low_pass_value);

#endif /* ROCKETLIB_LOW_PASS_FILTER_H */