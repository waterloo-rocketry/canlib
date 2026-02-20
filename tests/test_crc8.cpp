#include <cstdint>

#include "common.h"
#include "crc8.h"

#include "rockettest.hpp"

class crc8_test : rockettest_test {
public:
	crc8_test() : rockettest_test("crc8_test") {}

	bool run_test() override {
		bool test_passed = true;

		rockettest_check_assert_triggered(
			[] { crc8_checksum(nullptr, 0, 0); }); // nullptr reject with zero input size

		rockettest_check_assert_triggered(
			[] { crc8_checksum(nullptr, 5, 0); }); // nullptr reject with not-zero input size

		return test_passed;
	}
};

crc8_test crc8_test_inst;
