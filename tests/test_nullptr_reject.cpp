#include "rockettest.hpp"

#include "message/msg_common.h"

class msg_ptr_nullptr_reject_test : rockettest_test {
public:
	msg_ptr_nullptr_reject_test() : rockettest_test("msg_ptr_nullptr_reject_test") {}

	bool run_test() override {
		bool test_failed = false;

		test_failed |= rockettest_check_assert_trigger([] { get_message_type(nullptr); });
		test_failed |= rockettest_check_assert_trigger([] { get_board_type_unique_id(nullptr); });
		test_failed |= rockettest_check_assert_trigger([] { get_board_inst_unique_id(nullptr); });
		test_failed |= rockettest_check_assert_trigger([] { get_message_metadata(nullptr); });
		test_failed |= rockettest_check_assert_trigger([] { write_timestamp(0, nullptr); });
		test_failed |= rockettest_check_assert_trigger([] { get_timestamp(nullptr); });

		return !test_failed;
	}
};

msg_ptr_nullptr_reject_test msg_ptr_nullptr_reject_test_inst;
