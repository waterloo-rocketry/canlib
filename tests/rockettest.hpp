#ifndef ROCKETTEST_H
#define ROCKETTEST_H

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#define CONSOLE_COLOUR_RESET "\033[0m"
#define CONSOLE_COLOUR_RED "\033[1;31m"
#define CONSOLE_COLOUR_GREEN "\033[1;32m"

#define rockettest_assert(statement)                                                               \
	if (!(statement)) {                                                                            \
		std::cout << "Assertion failed " << __FILE__ << ':' << __LINE__ << ' ' << #statement       \
				  << std::endl;                                                                    \
		test_passed = false;                                                                       \
	}

template <typename T, std::uint32_t mask = 0xffffffff> T rockettest_rand() {
	return static_cast<T>(std::rand() & mask);
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

#endif
