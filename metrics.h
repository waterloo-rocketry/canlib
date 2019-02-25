#include <xc.h>
#include <stdint.h>

#include "can.h"

/*
 * This file provides some utilities for tracking performance metrics for
 * CAN communication. Potentially important metrics that should be tracked:
 * - transmissions
 * - transmission errors
 * - message receptions
 * - message_reception errors
 * - time since last error of x type
 * - average time between errors of x type
 *
 * These functions should be called from within the device-specific CAN drivers.
 * They are intended for stress testing and should be surrounded by ifdefs to
 * minimize impact on real-world performance.
 */

// All time values are unit values - use whatever the host app is using

// number of attempted transmissions, successful or otherwise
void inc_tx_count(void);
// number of erroneous message transmissions
void inc_tx_error_count(uint32_t current_time);

void inc_rx_count(void);
void inc_rx_error_count(uint32_t current_time);
