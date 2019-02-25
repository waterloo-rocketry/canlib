#include "metrics.h"

static uint32_t tx_count = 0;
static uint32_t tx_error_count = 0;

static uint32_t rx_count = 0;
static uint32_t rx_error_count = 0;

// max and average time between TX/RX errors
static uint32_t tx_max_error_time = 0;
static uint32_t tx_avg_error_time = 0;

static uint32_t rx_max_error_time = 0;
static uint32_t rx_avg_error_time = 0;

// passed in by calling application, so it's a little unreliable
static uint32_t tx_last_error_time = 0;
static uint32_t rx_last_error_time = 0;

void inc_tx_count(void) {
    tx_count++;
}

void inc_tx_error_count(uint32_t current_time) {
    tx_error_count++;

    // update the max error time
    if (current_time - tx_last_error_time > tx_max_error_time) {
        tx_max_error_time = current_time - tx_last_error_time;
    }

    // update the average
    tx_avg_error_time = current_time / tx_error_count;
    tx_last_error_time = current_time;
}

void inc_rx_count(void) {
    rx_count++;
}

void inc_rx_error_count(uint32_t current_time) {
    rx_error_count++;

    // update the max error time
    if (current_time - rx_last_error_time > rx_max_error_time) {
        rx_max_error_time = current_time - rx_last_error_time;
    }

    // update the average
    rx_avg_error_time = current_time / rx_error_count;
    rx_last_error_time = current_time;
}

