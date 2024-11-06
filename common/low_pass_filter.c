#include "low_pass_filter.h"
#include <stddef.h>

// #include <xc.h>

// Initialize the low-pass filter
w_status_t low_pass_filter_init(double *alpha, double response_time) {
    if (alpha == NULL || response_time <= 0) {
        return W_INVALID_PARAM; // Return specific error for invalid parameters
    }
    // Calculate and store the alpha value using the LOW_PASS_ALPHA macro
    *alpha = LOW_PASS_ALPHA(response_time, response_time);
    return W_SUCCESS; // Return success after initialization
}

// Update the low-pass filter with a new value
w_status_t update_low_pass(double *alpha, uint16_t new_value, double *low_pass_value) {
    // Check if alpha or low_pass_value pointers are NULL
    if (alpha == NULL || low_pass_value == NULL) {
        return W_INVALID_PARAM; // Return specific error for invalid parameters
    }

    // Ensure alpha is within the valid range
    if (*alpha <= 0.0 || *alpha >= 1.0) {
        return W_INVALID_PARAM;
    }

    // Low pass filter formula: y[n] = alpha * x[n] + (1 - alpha) * y[n-1]
    *low_pass_value = (*alpha * new_value) + ((1.0 - *alpha) * (*low_pass_value));
    return W_SUCCESS; // Return success after successful update
}