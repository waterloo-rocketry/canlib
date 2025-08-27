#ifndef ROCKETLIB_LOW_PASS_FILTER_H
#define ROCKETLIB_LOW_PASS_FILTER_H

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// Inline function to calculate the sample frequency based on the time difference in milliseconds
static inline double sample_freq(double time_diff_ms) {
	return 1000.0 / time_diff_ms;
}

// Inline function to calculate the alpha value for the low-pass filter based on the response time
static inline double low_pass_alpha(double TR, double time_diff_ms) {
	double freq = sample_freq(time_diff_ms);
	return (freq * TR / 5.0) / (1.0 + freq * TR / 5.0);
}

// Initializes the low-pass filter by calculating and storing the alpha value based on the given
// response time.
w_status_t low_pass_filter_init(double *alpha, double response_time);

// Updates the low-pass filter with a new value and returns the operation status
w_status_t update_low_pass(double alpha, uint16_t new_input_value, double *low_pass_value);

#ifdef __cplusplus
}
#endif

#endif /* ROCKETLIB_LOW_PASS_FILTER_H */
