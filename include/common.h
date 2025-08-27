#ifndef ROCKETLIB_COMMON_H
#define ROCKETLIB_COMMON_H

// Rocketlib Version
#define ROCKETLIB_VERSION_MAJOR 2025
#define ROCKETLIB_VERSION_MINOR 1

// Standard Error Code
typedef enum {
	W_SUCCESS = 0,
	W_FAILURE,
	W_INVALID_PARAM,
	W_IO_ERROR,
	W_IO_TIMEOUT,
	W_MATH_ERROR,
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
