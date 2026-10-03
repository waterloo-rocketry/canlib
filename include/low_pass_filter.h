/**
 * @file
 * @brief Low-pass filter implementation
 *
 * This module provides a low-pass filter implementation for smoothing sensor readings
 * and reducing noise in analog signals. The filter uses an exponential moving average
 * algorithm with configurable response time.
 */

#ifndef ROCKETLIB_LOW_PASS_FILTER_H
#define ROCKETLIB_LOW_PASS_FILTER_H

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculate the sample frequency based on the time difference in milliseconds
 *
 * @param time_diff_ms Time difference between samples in milliseconds
 * @return double Sample frequency in Hz
 */
static inline double sample_freq(double time_diff_ms) {
	return 1000.0 / time_diff_ms;
}

/**
 * @brief Calculate the alpha value for the low-pass filter based on the response time
 *
 * The alpha value determines the filter's responsiveness. A higher alpha value results
 * in faster response but less filtering, while a lower alpha value provides more filtering
 * but slower response.
 *
 * @param TR Response time constant in milliseconds
 * @param time_diff_ms Time difference between samples in milliseconds
 * @return double Alpha value (0.0 to 1.0) for the low-pass filter
 */
static inline double low_pass_alpha(double TR, double time_diff_ms) {
	double freq = sample_freq(time_diff_ms);
	return (freq * TR / 5.0) / (1.0 + freq * TR / 5.0);
}

/**
 * @brief Initializes the low-pass filter by calculating and storing the alpha value
 *
 * This function calculates the alpha value based on the given response time and stores
 * it in the provided pointer. The alpha value is used in subsequent filter updates.
 *
 * @param alpha Pointer to store the calculated alpha value
 * @param response_time Response time constant in milliseconds (must be > 0)
 * @return w_status_t Returns W_SUCCESS on success, W_INVALID_PARAM if alpha is NULL or
 * response_time is invalid
 */
w_status_t low_pass_filter_init(double *alpha, double response_time);

/**
 * @brief Updates the low-pass filter with a new value and returns the operation status
 *
 * This function applies the low-pass filter algorithm to the new input value using the
 * provided alpha value. The filtered result is stored in low_pass_value and updated
 * in-place for the next iteration.
 *
 * @param alpha Alpha value (0.0 to 1.0) for the low-pass filter
 * @param new_input_value New input value to filter
 * @param low_pass_value Pointer to the current filtered value (updated in-place)
 * @return w_status_t Returns W_SUCCESS on success, W_INVALID_PARAM if low_pass_value is
 * NULL or alpha is out of valid range (0.0 to 1.0)
 */
w_status_t update_low_pass(double alpha, uint16_t new_input_value, double *low_pass_value);

#ifdef __cplusplus
}
#endif

#endif /* ROCKETLIB_LOW_PASS_FILTER_H */
