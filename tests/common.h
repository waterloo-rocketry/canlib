#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

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
