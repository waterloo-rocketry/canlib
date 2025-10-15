#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "low_pass_filter.h"

#include "rockettest.hpp"

#define EPSILON 0.0001

class low_pass_filter_init_test : rockettest_test {
public:
	low_pass_filter_init_test() : rockettest_test("low_pass_filter_init_test") {}

	bool run_test() override {
		bool test_passed = true;

		double alpha;
		// Test with valid input
		rockettest_assert(low_pass_filter_init(&alpha, 1.0) == W_SUCCESS);
		// Test with invalid input (NULL pointer)
		rockettest_assert(low_pass_filter_init(NULL, 1.0) == W_INVALID_PARAM);
		// Test with invalid input (negative response time)
		rockettest_assert(low_pass_filter_init(&alpha, -1.0) == W_INVALID_PARAM);
		// Test with invalid input (zero response time)
		rockettest_assert(low_pass_filter_init(&alpha, 0.0) == W_INVALID_PARAM);

		return test_passed;
	}
};

low_pass_filter_init_test low_pass_filter_init_test_inst;

class low_pass_filter_update_test : rockettest_test {
public:
	low_pass_filter_update_test() : rockettest_test("low_pass_filter_update_test") {}

	bool run_test() override {
		bool test_passed = true;

		double alpha = 0.1; // Smoothing factor (0 < alpha < 1)
		double low_pass_value = 0.0; // Initial low-pass filtered value
		uint16_t new_input_value = 100; // Fixed input value to test behavior
		int iterations = 50; // Number of times to apply the filter
		w_status_t result;

		printf("Testing Low-Pass Filter with Fixed Input\n");
		printf("-----------------------------------------\n");
		printf("Alpha (smoothing factor): %.2f\n", alpha);
		printf("Input Value: %u\n\n", new_input_value);

		for (int i = 1; i <= iterations; ++i) {
			// Store the previous low_pass_value before updating
			double previous_low_pass = low_pass_value;
			// Calculate the expected value based on the low-pass filter formula
			double expected = (alpha * new_input_value) + ((1.0 - alpha) * previous_low_pass);
			// Update the low-pass filter with the fixed input value
			result = update_low_pass(alpha, new_input_value, &low_pass_value);
			rockettest_assert(result == W_SUCCESS);
			// Print the current iteration and values
			printf("Iteration %2d: Low-Pass Value = %.4f | Expected ~ %.4f\n",
				   i,
				   low_pass_value,
				   expected);
			// Verify the correctness of the low-pass update
			rockettest_assert(fabs(low_pass_value - expected) < EPSILON);
		}

		printf("\nAll iterations completed successfully.\n\n");

		// Test with alpha out of range
		result = update_low_pass(-0.1, new_input_value, &low_pass_value);
		rockettest_assert(result == W_INVALID_PARAM);
		printf("Invalid alpha (negative) test passed.\n");

		result = update_low_pass(1.1, new_input_value, &low_pass_value);
		rockettest_assert(result == W_INVALID_PARAM);
		printf("Invalid alpha (greater than 1) test passed.\n");

		// Test with NULL low_pass_value
		result = update_low_pass(alpha, new_input_value, NULL);
		rockettest_assert(result == W_INVALID_PARAM);
		printf("Null low_pass_value test passed.\n");

		return test_passed;
	}
};

low_pass_filter_update_test low_pass_filter_update_test_inst;
