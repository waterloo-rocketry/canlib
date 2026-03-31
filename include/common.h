#ifndef ROCKETLIB_COMMON_H
#define ROCKETLIB_COMMON_H

#include <stdint.h>

/// @brief Rocketlib major release number, shall be design cycle year
#define ROCKETLIB_VERSION_MAJOR 2026

/// @brief Rocketlib minor release number, shall be the Nth release of the design cycle
#define ROCKETLIB_VERSION_MINOR 1

/// @brief Standard error code
typedef enum {
	/// @brief Success
	W_SUCCESS = 0,
	/// @brief Failure, if not one of the more specific case below
	W_FAILURE,
	/// @brief Illegal function call arguments/parameters
	W_INVALID_PARAM,
	/// @brief IO error, for example no SD card was plugged in
	W_IO_ERROR,
	/// @brief IO timeout, for example SD card takes too long to respond
	W_IO_TIMEOUT,
	/// @brief Math error, for example divide by zero
	W_MATH_ERROR,
	/// @brief Buffer overflow, for example trying to write to a full FIFO
	W_OVERFLOW
} w_status_t;

#ifndef UNIT_TEST

// Assertion Macro
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Assertion fail handler
 *
 * Assertion fail handler, shall be implemented in the firmware, only used if `W_DEBUG` is defined
 *
 * @param file Path to C source file contains assertion failure
 * @param line C source line number of assertion failure
 * @param statement Statement caused assertion failure
 */
void w_assert_fail(const char *file, int line, const char *statement);

#ifdef __cplusplus
}
#endif

#ifdef W_DEBUG

#define w_assert(statement)                                                                        \
	if (!(statement)) {                                                                            \
		w_assert_fail(__FILE__, __LINE__, #statement);                                             \
	}

#else

/**
 * @brief Assertion
 *
 * Works just like standard C assertion, except use can define custom assertion failure handler(see
 * `w_assert_fail`), only active when `W_DEBUG` is defined
 */
#define w_assert(statement)

#endif

#else

#ifdef __cplusplus

extern void rocketlib_assert_pass_cpp(const char *file, int line, const char *statement);
extern void rocketlib_assert_fail_cpp(const char *file, int line, const char *statement);

#define w_assert(statement)                                                                        \
	if (statement) {                                                                               \
		rocketlib_assert_pass_cpp(__FILE__, __LINE__, #statement);                                 \
	} else {                                                                                       \
		rocketlib_assert_fail_cpp(__FILE__, __LINE__, #statement);                                 \
	}

#else

extern void rocketlib_assert_pass_c(const char *file, int line, const char *statement);
extern void rocketlib_assert_fail_c(const char *file, int line, const char *statement);

#define w_assert(statement)                                                                        \
	if (statement) {                                                                               \
		rocketlib_assert_pass_c(__FILE__, __LINE__, #statement);                                   \
	} else {                                                                                       \
		rocketlib_assert_fail_c(__FILE__, __LINE__, #statement);                                   \
	}

#endif

#endif

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
static inline float value_clamp_float(float x, float low, float high) {
	if (x < low) {
		return low;
	} else if (x > high) {
		return high;
	} else {
		return x;
	}
}

#endif
