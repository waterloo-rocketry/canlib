#ifndef MATHOPS_H
#define MATHOPS_H

#include <stdint.h>

#include "common.h"

/**
 * @brief Clamps a 32 bit unsigned integer between a lower and upper bound.
 *
 * @param x The value to clamp
 * @param low The lower bound, inclusive
 * @param high The upper bound, inclusive
 * @return The clamped value
 */
static inline uint32_t value_clamp_uint32(uint32_t x, uint32_t low, uint32_t high) {
	if (x < low) {
		return low;
	} else if (x > high) {
		return high;
	} else {
		return x;
	}
}

/**
 * @brief Clamps a 32 bit signed integer between a lower and upper bound.
 *
 * @param x The value to clamp
 * @param low The lower bound, inclusive
 * @param high The upper bound, inclusive
 * @return The clamped value
 */
static inline int32_t value_clamp_int32(int32_t x, int32_t low, int32_t high) {
	if (x < low) {
		return low;
	} else if (x > high) {
		return high;
	} else {
		return x;
	}
}

/**
 * @brief Clamps a 32 bit float between a lower and upper bound.
 *
 * @param x The value to clamp
 * @param low The lower bound, inclusive
 * @param high The upper bound, inclusive
 * @return The clamped value
 */
static inline float32_t value_clamp_float32(float32_t x, float32_t low, float32_t high) {
	if (x < low) {
		return low;
	} else if (x > high) {
		return high;
	} else {
		return x;
	}
}

#endif
