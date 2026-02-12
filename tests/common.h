#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

#ifdef __cplusplus

extern void rocketlib_assert_result_cpp(const char *file, int line, const char *statement,
										bool result);

#define w_assert(statement)                                                                        \
	rocketlib_assert_result_cpp(__FILE__, __LINE__, #statement, (statement));

#else

extern void rocketlib_assert_result_c(const char *file, int line, const char *statement,
									  bool result);

#define w_assert(statement) rocketlib_assert_result_c(__FILE__, __LINE__, #statement, (statement));

#endif

#endif
