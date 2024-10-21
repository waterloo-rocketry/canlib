#include "low_pass_filter.h"

// Initialize the low-pass filter
w_status_t low_pass_filter_init(double *alpha, double response_time) {
    if (alpha == NULL || response_time <= 0) {
        return W_FAILURE; // Return failure if the response time is invalid or alpha is NULL
    }
    *alpha = LOW_PASS_ALPHA(response_time); // Calculate and store the alpha value
    return W_SUCCESS; // Return success
}
// Update the low-pass filter with a new value
uint16_t update_low_pass(double *alpha, uint16_t new_value, double *low_pass_value) {
    if (alpha == NULL || low_pass_value == NULL) {
        return 0; // Return 0 if alpha or low_pass_value is NULL
    }

    // Update the low-pass filter value
    *low_pass_value = *alpha * (*low_pass_value) + (1.0 - *alpha) * new_value;
    return (uint16_t)(*low_pass_value); // Return the updated low-pass filter value
}