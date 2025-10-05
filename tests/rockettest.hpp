/*
 * Rockettest - Class-based unit testing framework for CAN message testing
 *
 * Features:
 * - Auto-registration: Tests register themselves at startup
 * - Class-based: Each test is a class inheriting from rockettest_test
 * - Random testing: Built-in random data generation with seed control
 * - Colored output: Pass/fail results with ANSI color codes
 */

#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>

// ANSI color codes for test output
#define CONSOLE_COLOUR_RESET "\033[0m"
#define CONSOLE_COLOUR_RED "\033[1;31m"
#define CONSOLE_COLOUR_GREEN "\033[1;32m"

/*
 * Test assertion macro
 * Evaluates a boolean statement and reports pass/fail with location info.
 * Sets test_passed = false on failure (must be defined in test's run_test())
 */
#define rockettest_assert(statement)                                                               \
	if (!(statement)) {                                                                            \
		std::cout << "Assertion failed " << __FILE__ << ':' << __LINE__ << ' ' << #statement       \
				  << std::endl;                                                                    \
		test_passed = false;                                                                       \
	}

/*
 * Generate random test data with optional bit mask
 * Template parameters:
 *   T - Type to generate (e.g., uint16_t, can_msg_prio_t)
 *   mask - Bit mask to limit range (default 0xffffffff = no limit)
 * Example: rockettest_rand<uint8_t, 0x3>() generates 0-3
 */
template <typename T, std::uint32_t mask = 0xffffffff> T rockettest_rand() {
	return static_cast<T>(std::rand() & mask);
}

/*
 * Base class for all unit tests
 *
 * Usage:
 * 1. Create a class inheriting from rockettest_test
 * 2. Pass test name to constructor
 * 3. Implement run_test() with your test logic
 * 4. Create a global instance - it auto-registers
 *
 * Example:
 *   class my_test : rockettest_test {
 *   public:
 *       my_test() : rockettest_test("my_test") {}
 *       bool run_test() override {
 *           bool test_passed = true;
 *           rockettest_assert(1 + 1 == 2);
 *           return test_passed;
 *       }
 *   };
 *   my_test my_test_inst;  // Auto-registers
 */
class rockettest_test {
	const char *name;

public:
	// Registry of all tests (auto-populated at startup)
	static std::map<const char *, rockettest_test *> tests;

	rockettest_test() = delete;
	rockettest_test(const char *test_name);

	// Implement this in your test class
	virtual bool run_test() = 0;

	// Run the test and print results (called by test runner)
	bool operator()();
};

#endif
