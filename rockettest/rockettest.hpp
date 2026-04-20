#ifndef ROCKETTEST_H
#define ROCKETTEST_H

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <string>

/**
 * Base functionality
 */

#define CONSOLE_COLOUR_RESET "\033[0m"
#define CONSOLE_COLOUR_RED "\033[1;31m"
#define CONSOLE_COLOUR_GREEN "\033[1;32m"

#define rockettest_check_expr_true(expr)                                                           \
	if (!(expr)) {                                                                                 \
		std::cout << CONSOLE_COLOUR_RED << "E: " << CONSOLE_COLOUR_RESET                           \
				  << "Expression should be true " << __FILE__ << ':' << __LINE__ << ' ' << #expr   \
				  << std::endl;                                                                    \
		test_passed = false;                                                                       \
	}

#define rockettest_check_assert_triggered(funccall)                                                \
	if (!rockettest_check_assert_sjlj((funccall))) {                                               \
		std::cout << CONSOLE_COLOUR_RED << "E: " << CONSOLE_COLOUR_RESET                           \
				  << "Rocketlib w_assert did not trigger on " << __FILE__ << ':' << __LINE__       \
				  << ' ' << #funccall << std::endl;                                                \
		test_passed = false;                                                                       \
	}

class rockettest_test {
	const char *name;

public:
	static std::map<std::string, rockettest_test *> tests;

	rockettest_test() = delete;
	rockettest_test(const char *test_name);

	virtual bool run_test() = 0;

	bool operator()();
};

bool rockettest_check_assert_sjlj(std::function<void(void)> funccall);

/**
 * Random Number Generation
 */

template <typename T, std::uint32_t mask = 0xffffffff> T rockettest_rand_field() {
	return static_cast<T>(std::rand() & mask);
}

template <typename T, std::uint32_t mask = 0xffffffff>
T rockettest_rand_field_with_exclude(T exclude) {
	T rn = static_cast<T>(std::rand() & mask);
	if (rn == exclude) {
		rn = (rn + 1) & mask;
	}
	return rn;
}

// Generate a random number in [min,max)
template <typename T> T rockettest_rand_range(T min, T max) {
	return static_cast<T>((std::rand() % (static_cast<int>(max) - static_cast<int>(min))) +
						  static_cast<int>(min));
}

template <typename T> T rockettest_rand_range_with_exclude(T min, T max, T exclude) {
	T rn = static_cast<T>((std::rand() % (static_cast<int>(max) - static_cast<int>(min))) +
						  static_cast<int>(min));
	if (rn == exclude) {
		rn += 1;
		if (rn == max) {
			rn = min;
		}
	}
	return rn;
}

#endif
