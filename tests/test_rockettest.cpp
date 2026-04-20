#include <cstdint>
#include <stdint.h>

#include "rockettest.hpp"

class test_rand_field : rockettest_test {
public:
	test_rand_field() : rockettest_test("test_rand_field") {}

	bool run_test() override {
		bool test_passed = true;

		int dist[256]{};

		for (int i = 0; i < 10000; i++) {
			uint8_t randval = rockettest_rand_field<uint8_t>();
			dist[randval]++;
		}

		int min_occurrence = 10000;
		int max_occurrence = 0;

		for (int k : dist) {
			rockettest_check_expr_true(k >= 1);
			if (k > max_occurrence) {
				max_occurrence = k;
			}
			if (k < min_occurrence) {
				min_occurrence = k;
			}
		}

		std::cout << "Max Occurrence = " << max_occurrence << std::endl;
		std::cout << "Min Occurrence = " << min_occurrence << std::endl;

		return test_passed;
	}
};

test_rand_field test_rand_field_inst;

class test_rand_range : rockettest_test {
public:
	test_rand_range() : rockettest_test("test_rand_range") {}

	bool run_test() override {
		bool test_passed = true;

		int dist[100]{};

		for (int i = 0; i < 10000; i++) {
			uint8_t randval = rockettest_rand_range<uint8_t>(100, 200);
			rockettest_check_expr_true(randval >= 100);
			rockettest_check_expr_true(randval < 200);
			dist[randval - 100]++;
		}

		int min_occurrence = 10000;
		int max_occurrence = 0;

		for (int k : dist) {
			rockettest_check_expr_true(k >= 1);
			if (k > max_occurrence) {
				max_occurrence = k;
			}
			if (k < min_occurrence) {
				min_occurrence = k;
			}
		}

		std::cout << "Max Occurrence = " << max_occurrence << std::endl;
		std::cout << "Min Occurrence = " << min_occurrence << std::endl;

		return test_passed;
	}
};

test_rand_range test_rand_range_inst;
