#include "rockettest.hpp"

#include "message/msg_common.h"
#include "util/can_rcv_buffer.h"
#include "util/can_tx_buffer.h"
#include "util/safe_ring_buffer.h"

class msg_ptr_nullptr_reject_test : rockettest_test {
public:
	msg_ptr_nullptr_reject_test() : rockettest_test("msg_ptr_nullptr_reject_test") {}

	bool run_test() override {
		bool test_passed = true;

		// 	msg_common tests
		rockettest_check_assert_triggered([] { get_message_type(nullptr); });
		rockettest_check_assert_triggered([] { get_board_type_unique_id(nullptr); });
		rockettest_check_assert_triggered([] { get_board_inst_unique_id(nullptr); });
		rockettest_check_assert_triggered([] { get_message_metadata(nullptr); });
		rockettest_check_assert_triggered([] { write_timestamp(0, nullptr); });
		rockettest_check_assert_triggered([] { get_timestamp(nullptr); });

		// msg_actuator tests
		// TODO

		// msg_general tests
		// TODO

		// msg_gps tests
		// TODO

		// msg_recovery tests
		// TODO

		// msg_sensor tests
		// TODO

		// msg_stream tests
		// TODO

		// safe_ring_buffer tests
		rockettest_check_assert_triggered([] { srb_init(nullptr, nullptr, 0, 0); });
		rockettest_check_assert_triggered([] { srb_push(nullptr, nullptr); });
		rockettest_check_assert_triggered([] { srb_is_full(nullptr); });
		rockettest_check_assert_triggered([] { srb_is_empty(nullptr); });
		rockettest_check_assert_triggered([] { srb_pop(nullptr, nullptr); });
		rockettest_check_assert_triggered([] { srb_peek(nullptr, nullptr); });

		// can_tx_buffer tests
		rockettest_check_assert_triggered([] { txb_init(nullptr, 0, nullptr, nullptr); });
		rockettest_check_assert_triggered([] { txb_enqueue(nullptr); });

		// can_rx_buffer tests
		rockettest_check_assert_triggered([] { rcvb_init(nullptr, 0); });
		rockettest_check_assert_triggered([] { rcvb_push_message(nullptr); });
		rockettest_check_assert_triggered([] { rcvb_pop_message(nullptr); });
		rockettest_check_assert_triggered([] { rcvb_peek_message(nullptr); });

		return test_passed;
	}
};

msg_ptr_nullptr_reject_test msg_ptr_nullptr_reject_test_inst;
