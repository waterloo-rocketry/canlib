#include <cstdlib>
#include <ctime>
#include <iostream>

#include "test_common.hpp"

int main(void) {
	int seed = std::time({});
	std::cout << "Random seed " << seed << std::endl;
	std::srand(seed);

	bool all_passed = true;

	for (rocketry_test *rt : rocketry_test::tests) {
		if (!(*rt)()) {
			all_passed = false;
		}
	}

	if(all_passed){
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;	
}

std::vector<rocketry_test *> rocketry_test::tests;

rocketry_test::rocketry_test() {
	tests.push_back(this);
}

bool rocketry_test::operator()() {
	std::cout << "Running " << get_name() << std::endl;
	bool test_result = run_test();
	if (test_result == true) {
		std::cout << CONSOLE_COLOUR_GREEN << "PASSED " << CONSOLE_COLOUR_RESET << get_name()
				  << std::endl;
	} else {
		std::cout << CONSOLE_COLOUR_RED << "FAILED " << CONSOLE_COLOUR_RESET << get_name()
				  << std::endl;
	}
	return test_result;
}
