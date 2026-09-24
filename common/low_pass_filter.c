#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "low_pass_filter.h"

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
w_status_t low_pass_filter_init(double *alpha, double response_time) {
	if (alpha == NULL || response_time <= 0) {
		return W_INVALID_PARAM; // Return specific error for invalid parameters
	}
	// Calculate and store the alpha value using the low_pass_alpha function
	*alpha = low_pass_alpha(response_time, response_time);
	return W_SUCCESS; // Return success after initialization
}

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
w_status_t update_low_pass(double alpha, uint16_t new_input_value, double *low_pass_value) {
	// Check if low_pass_value pointer is NULL
	if (low_pass_value == NULL) {
		return W_INVALID_PARAM; // Return specific error for invalid parameters
	}

	// Ensure alpha is within the valid range
	if (alpha <= 0.0 || alpha >= 1.0) {
		return W_INVALID_PARAM;
	}

	// Low pass filter formula: y[n] = alpha * x[n] + (1 - alpha) * y[n-1]
	*low_pass_value = (alpha * new_input_value) + ((1.0 - alpha) * (*low_pass_value));
	return W_SUCCESS; // Return success after successful update
}
