#include "low_pass_filter.h"

static volatile double alpha_low;

status_t low_pass_filter_init(double response_time) {
    if (response_time <= 0) {
        return W_FAILURE;
    }
    alpha_low = LOW_PASS_ALPHA(response_time);
    return W_SUCCESS;
}

uint16_t update_low_pass(uint16_t new_value, double *low_pass_value) {
    if (low_pass_value == NULL) {
        return 0;
    }

    *low_pass_value = alpha_low * (*low_pass_value) + (1.0 - alpha_low) * new_value;
    return (uint16_t)(*low_pass_value);
}
