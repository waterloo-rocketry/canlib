#include <cstdlib>
#include <ctime>
#include <iostream>

#include "test_msg_sensor.hpp"

int main(void) {
	int seed = std::time({});
	std::cout << "Random seed " << seed << std::endl;
	std::srand(seed);

	test_temp_data_msg();
	test_analog_sensor_msg();

	return 0;
}
