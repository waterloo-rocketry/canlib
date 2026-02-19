#include "rockettest.hpp"

#include "message/msg_common.h"

class msg_ptr_nullptr_reject_test : rockettest_test {
public:
	msg_ptr_nullptr_reject_test() : rockettest_test("msg_ptr_nullptr_reject_test") {}

	bool run_test() override {
		bool test_passed = true;

		rockettest_check_assert_triggered([] { get_message_type(nullptr); });
		rockettest_check_assert_triggered([] { get_board_type_unique_id(nullptr); });
		rockettest_check_assert_triggered([] { get_board_inst_unique_id(nullptr); });
		rockettest_check_assert_triggered([] { get_message_metadata(nullptr); });
		rockettest_check_assert_triggered([] { write_timestamp(0, nullptr); });
		rockettest_check_assert_triggered([] { get_timestamp(nullptr); });

		return test_passed;
	}
};

msg_ptr_nullptr_reject_test msg_ptr_nullptr_reject_test_inst;
