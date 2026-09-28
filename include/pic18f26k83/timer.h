/**
 * @file
 * @brief PIC18 Timer driver provides millis function
 *
 * This module provides basic timer functionalities, such as initializing Timer0, handling
 * interrupts, and tracking milliseconds since the timer was started.
 */

#ifndef ROCKETLIB_TIMER_H
#define ROCKETLIB_TIMER_H

#include <stdint.h>

#ifdef __cplusplus
#error "C++ is not supported"
#endif

/**
 * @brief Initializes Timer0 for time tracking. This function must be called before using any other
 * timer-related functionality.
 *
 * - Enables Timer0 interrupt.
 * - Sets the timer to 8-bit mode.
 * - Driven by a 500 kHz clock.
 * - Ensures the timer is synchronized to the system clock.
 */
void timer0_init(void);

/**
 * @brief Function should be called from main ISR when Timer0 interrupt is triggered
 *
 * This function should be called from the main Interrupt Service Routine (ISR) whenever the Timer0
 * interrupt is triggered. It updates the internal time counter.
 *
 * @warning This function does **not** clear the interrupt flag (`PIR3bits.TMR0IF`), This is the
 * responsibility of the top level ISR.
 */
void timer0_handle_interrupt(void);

/**
 * @brief Returns the number of milliseconds since `timer0_init()` was called
 *
 * Returns the number of milliseconds since `timer0_init()` was called. This is a safe function that
 * disables interrupts momentarily to return a consistent value.
 */
uint32_t millis(void);

#endif /* ROCKETLIB_TIMER_H */
