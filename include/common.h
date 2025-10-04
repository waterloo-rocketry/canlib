#ifndef ROCKETLIB_COMMON_H
#define ROCKETLIB_COMMON_H

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
	/// @brief IO timeout, for example SD car takes too long to respond
	W_IO_TIMEOUT,
	/// @brief Math error, for example divide by zero
	W_MATH_ERROR,
	/// @brief Buffer overflow, for example trying to write to a full FIFO
	W_OVERFLOW
} w_status_t;

// Assertion Macro
#ifdef __cplusplus
extern "C" {
#endif

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

#define w_assert(statement)

#endif

#endif
