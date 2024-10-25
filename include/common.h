#ifndef ROCKETLIB_COMMON_H
#define ROCKETLIB_COMMON_H

typedef enum {
    W_SUCCESS = 0,
    W_FAILURE,
    W_INVALID_PARAM,
    W_IO_ERROR,
    W_IO_TIMEOUT,
    W_MATH_ERROR,
    W_OVERFLOW
} w_status_t;

void w_assert_fail(const char *file, int line, const char *statement);

#ifdef W_DEBUG

#define w_assert(statement)                                                                        \
    if (!(statement)) {                                                                            \
        w_assert_fail(__FILE__, __LINE__, #statement);                                             \
    }

#else

#define w_assert(statement)

#endif

#endif
