#include <cstdlib>
#include <ctime>
#include <getopt.h>
#include <iostream>

#include "rockettest.hpp"

int main(int argc, char *argv[]) {
	int seed = std::time({});

	int opt;
	while ((opt = getopt(argc, argv, "s:")) != -1) {
		switch (opt) {
			case 's':
				seed = atoi(optarg);
				break;

			default:
				std::cout << "Usage: unit_test [-s random_seed]" << std::endl;
				return EXIT_FAILURE;
		}
	}

	std::cout << "Random seed " << seed << std::endl;
	std::srand(seed);

	bool all_passed = true;

	for (auto &rt : rockettest_test::tests) {
		if (!(*rt.second)()) {
			all_passed = false;
		}
	}

	if (all_passed) {
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

std::map<const char *, rockettest_test *> rockettest_test::tests;

rockettest_test::rockettest_test(const char *test_name) : name(test_name) {
	tests[test_name] = this;
}

bool rockettest_test::operator()() {
	std::cout << "Running " << name << std::endl;
	bool test_result = run_test();
	if (test_result == true) {
		std::cout << CONSOLE_COLOUR_GREEN << "PASSED " << CONSOLE_COLOUR_RESET << name << std::endl;
	} else {
		std::cout << CONSOLE_COLOUR_RED << "FAILED " << CONSOLE_COLOUR_RESET << name << std::endl;
	}
	return test_result;
}
