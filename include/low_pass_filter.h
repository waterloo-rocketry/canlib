#ifndef ROCKETLIB_LOW_PASS_FILTER_H
#define ROCKETLIB_LOW_PASS_FILTER_H

#include "common.h"

// Macro to calculate the sample frequency based on the time difference in milliseconds
#define SAMPLE_FREQ(time_diff_ms) (1000.0 / (time_diff_ms))

// Macro to calculate the alpha value for the low-pass filter based on the response time
#define LOW_PASS_ALPHA(TR) ((SAMPLE_FREQ * TR / 5.0) / (1 + SAMPLE_FREQ * TR / 5.0))

// Initializes the low-pass filter by calculating the alpha value based on the response time
w_status_t low_pass_filter_init(double *alpha, double response_time);

// Updates the low-pass filter with a new value and returns the updated filter value
uint16_t update_low_pass(double *alpha, uint16_t new_value, double *low_pass_value);

// Updates the low-pass filter with a new value and returns the updated filter value
uint16_t update_low_pass(double *alpha, uint16_t new_value, double *low_pass_value);

#endif /* ROCKETLIB_LOW_PASS_FILTER_H */