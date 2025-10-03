#include <cstdlib>
#include <ctime>
#include <iostream>

#include "test_common.hpp"

int main(void) {
	int seed = std::time({});
	std::cout << "Random seed " << seed << std::endl;
	std::srand(seed);

	bool all_passed = true;

	for (auto &rt : rocketry_test::tests) {
		if (!(*rt.second)()) {
			all_passed = false;
		}
	}

	if (all_passed) {
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

std::map<const char *, rocketry_test *> rocketry_test::tests;

rocketry_test::rocketry_test(const char *test_name) : name(test_name) {
	tests[test_name] = this;
}

bool rocketry_test::operator()() {
	std::cout << "Running " << name << std::endl;
	bool test_result = run_test();
	if (test_result == true) {
		std::cout << CONSOLE_COLOUR_GREEN << "PASSED " << CONSOLE_COLOUR_RESET << name << std::endl;
	} else {
		std::cout << CONSOLE_COLOUR_RED << "FAILED " << CONSOLE_COLOUR_RESET << name << std::endl;
	}
	return test_result;
}
