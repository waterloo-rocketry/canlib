#include <cstdlib>
#include <ctime>
#include <iostream>

#include "test_msg_sensor.hpp"

bool test_failed = false;

int main(void) {
	int seed = std::time({});
	std::cout << "Random seed " << seed << std::endl;
	std::srand(seed);

	test_temp_data_msg();
	test_analog_sensor_msg();

	if (test_failed) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
