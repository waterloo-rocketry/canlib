#include <cmath>
#include <cstdint>
#include <cstdio>
#include <limits>

#include "common.h"
#include "mathops.h"

#include "rockettest.hpp"

class value_clamp_uint32_test : rockettest_test {
public:
	value_clamp_uint32_test() : rockettest_test("value_clamp_uint32_test") {}

	bool run_test() override {
		bool test_passed = true;

		printf("value_clamp_uint32: in-range and boundary behavior\n");
		rockettest_check_expr_true(value_clamp_uint32(50u, 10u, 100u) == 50u);
		rockettest_check_expr_true(value_clamp_uint32(10u, 10u, 100u) == 10u);
		rockettest_check_expr_true(value_clamp_uint32(100u, 10u, 100u) == 100u);

		printf("value_clamp_uint32: out-of-range behavior\n");
		rockettest_check_expr_true(value_clamp_uint32(9u, 10u, 100u) == 10u);
		rockettest_check_expr_true(value_clamp_uint32(101u, 10u, 100u) == 100u);

		printf("value_clamp_uint32: extreme values\n");
		rockettest_check_expr_true(value_clamp_uint32(0u, 1u, 2u) == 1u);
		rockettest_check_expr_true(
			value_clamp_uint32(std::numeric_limits<uint32_t>::max(), 1u, 2u) == 2u);
		rockettest_check_expr_true(
			value_clamp_uint32(0u, 0u, std::numeric_limits<uint32_t>::max()) == 0u);
		rockettest_check_expr_true(value_clamp_uint32(std::numeric_limits<uint32_t>::max(),
													  0u,
													  std::numeric_limits<uint32_t>::max()) ==
								   std::numeric_limits<uint32_t>::max());

		printf("value_clamp_uint32: interval with zero width\n");
		rockettest_check_expr_true(value_clamp_uint32(0u, 42u, 42u) == 42u);
		rockettest_check_expr_true(value_clamp_uint32(42u, 42u, 42u) == 42u);
		rockettest_check_expr_true(value_clamp_uint32(100u, 42u, 42u) == 42u);

		return test_passed;
	}
};

value_clamp_uint32_test value_clamp_uint32_test_inst;

class value_clamp_int32_test : rockettest_test {
public:
	value_clamp_int32_test() : rockettest_test("value_clamp_int32_test") {}

	bool run_test() override {
		bool test_passed = true;

		printf("value_clamp_int32: in-range and boundary behavior\n");
		rockettest_check_expr_true(value_clamp_int32(0, -10, 10) == 0);
		rockettest_check_expr_true(value_clamp_int32(-10, -10, 10) == -10);
		rockettest_check_expr_true(value_clamp_int32(10, -10, 10) == 10);

		printf("value_clamp_int32: out-of-range behavior\n");
		rockettest_check_expr_true(value_clamp_int32(-11, -10, 10) == -10);
		rockettest_check_expr_true(value_clamp_int32(11, -10, 10) == 10);

		printf("value_clamp_int32: extreme values\n");
		rockettest_check_expr_true(value_clamp_int32(std::numeric_limits<int32_t>::min(), -5, 5) ==
								   -5);
		rockettest_check_expr_true(value_clamp_int32(std::numeric_limits<int32_t>::max(), -5, 5) ==
								   5);
		rockettest_check_expr_true(value_clamp_int32(std::numeric_limits<int32_t>::min(),
													 std::numeric_limits<int32_t>::min(),
													 std::numeric_limits<int32_t>::max()) ==
								   std::numeric_limits<int32_t>::min());
		rockettest_check_expr_true(value_clamp_int32(std::numeric_limits<int32_t>::max(),
													 std::numeric_limits<int32_t>::min(),
													 std::numeric_limits<int32_t>::max()) ==
								   std::numeric_limits<int32_t>::max());

		printf("value_clamp_int32: interval with zero width\n");
		rockettest_check_expr_true(value_clamp_int32(-100, 7, 7) == 7);
		rockettest_check_expr_true(value_clamp_int32(7, 7, 7) == 7);
		rockettest_check_expr_true(value_clamp_int32(100, 7, 7) == 7);

		return test_passed;
	}
};

value_clamp_int32_test value_clamp_int32_test_inst;

class value_clamp_float_test : rockettest_test {
public:
	value_clamp_float_test() : rockettest_test("value_clamp_float_test") {}

	bool run_test() override {
		bool test_passed = true;

		printf("value_clamp_float32: in-range and boundary behavior\n");
		rockettest_check_expr_true(value_clamp_float32(0.5f, -1.0f, 1.0f) == 0.5f);
		rockettest_check_expr_true(value_clamp_float32(-1.0f, -1.0f, 1.0f) == -1.0f);
		rockettest_check_expr_true(value_clamp_float32(1.0f, -1.0f, 1.0f) == 1.0f);

		printf("value_clamp_float32: out-of-range behavior\n");
		rockettest_check_expr_true(value_clamp_float32(-2.0f, -1.0f, 1.0f) == -1.0f);
		rockettest_check_expr_true(value_clamp_float32(2.0f, -1.0f, 1.0f) == 1.0f);

		printf("value_clamp_float32: finite extreme values\n");
		rockettest_check_expr_true(
			value_clamp_float32(std::numeric_limits<float32_t>::lowest(), -1.0f, 1.0f) == -1.0f);
		rockettest_check_expr_true(
			value_clamp_float32(std::numeric_limits<float32_t>::max(), -1.0f, 1.0f) == 1.0f);

		printf("value_clamp_float32: infinity behavior\n");
		rockettest_check_expr_true(value_clamp_float32(-std::numeric_limits<float32_t>::infinity(),
													   -10.0f,
													   10.0f) == -10.0f);
		rockettest_check_expr_true(value_clamp_float32(std::numeric_limits<float32_t>::infinity(),
													   -10.0f,
													   10.0f) == 10.0f);

		printf("value_clamp_float32: NaN behavior\n");
		const float32_t nan_value = std::numeric_limits<float32_t>::quiet_NaN();
		rockettest_check_expr_true(std::isnan(value_clamp_float32(nan_value, -1.0f, 1.0f)));

		printf("value_clamp_float32: interval with zero width\n");
		rockettest_check_expr_true(value_clamp_float32(-5.0f, 3.5f, 3.5f) == 3.5f);
		rockettest_check_expr_true(value_clamp_float32(3.5f, 3.5f, 3.5f) == 3.5f);
		rockettest_check_expr_true(value_clamp_float32(9.0f, 3.5f, 3.5f) == 3.5f);

		return test_passed;
	}
};

value_clamp_float_test value_clamp_float_test_inst;
