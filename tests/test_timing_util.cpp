#include "common.h"
#include "rockettest.hpp"

#include "can.h"
#include "util/timing_util.h"

class timing_util_test : public rockettest_test {
public:
	timing_util_test() : rockettest_test("timing_util_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_timing_t timing;

		// 48 MHz, for example PIC18 with 12 MHz crystal and 4xPLL enabled
		rockettest_check_expr_true(can_generate_timing_params(48000000, &timing) == W_SUCCESS);
		rockettest_check_expr_true(timing.brp == 7);
		rockettest_check_expr_true(timing.sjw == 3);
		rockettest_check_expr_true(timing.btlmode == 1);
		rockettest_check_expr_true(timing.sam == 0);
		rockettest_check_expr_true(timing.seg1ph == 4);
		rockettest_check_expr_true(timing.prseg == 0);
		rockettest_check_expr_true(timing.seg2ph == 4);

		// 12 MHz, for example PIC18 with 12 MHz crystal and 4xPLL disabled
		rockettest_check_expr_true(can_generate_timing_params(12000000, &timing) == W_SUCCESS);
		rockettest_check_expr_true(timing.brp == 1);
		rockettest_check_expr_true(timing.sjw == 3);
		rockettest_check_expr_true(timing.btlmode == 1);
		rockettest_check_expr_true(timing.sam == 0);
		rockettest_check_expr_true(timing.seg1ph == 4);
		rockettest_check_expr_true(timing.prseg == 0);
		rockettest_check_expr_true(timing.seg2ph == 4);

		// 6 MHz, currently not used on physical hardware
		rockettest_check_expr_true(can_generate_timing_params(6000000, &timing) == W_SUCCESS);
		rockettest_check_expr_true(timing.brp == 0);
		rockettest_check_expr_true(timing.sjw == 3);
		rockettest_check_expr_true(timing.btlmode == 1);
		rockettest_check_expr_true(timing.sam == 0);
		rockettest_check_expr_true(timing.seg1ph == 4);
		rockettest_check_expr_true(timing.prseg == 0);
		rockettest_check_expr_true(timing.seg2ph == 4);

		// Invalid system frequency
		rockettest_check_expr_true(can_generate_timing_params(0, &timing) == W_INVALID_PARAM);

		return test_passed;
	}
};

timing_util_test timing_util_test_inst;
