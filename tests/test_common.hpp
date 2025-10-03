#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#define CONSOLE_COLOUR_RESET "\033[0m"
#define CONSOLE_COLOUR_RED "\033[1;31m"
#define CONSOLE_COLOUR_GREEN "\033[1;32m"

#define test_assert(statement)                                                                     \
	if (!(statement)) {                                                                            \
		std::cout << "Assertion failed " << __FILE__ << ':' << __LINE__ << ' ' << #statement       \
				  << std::endl;                                                                    \
		test_passed = false;                                                                       \
	}

template <typename T, std::uint32_t mask = 0xffffffff> T test_rand() {
	return static_cast<T>(std::rand() & mask);
}

class rocketry_test {
public:
	static std::vector<rocketry_test *> tests;

	rocketry_test();

	virtual bool run_test() = 0;
	virtual const char *get_name() = 0;

	bool operator()();
};

#endif
