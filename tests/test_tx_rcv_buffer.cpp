#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <queue>

#include "can.h"
#include "rockettest.hpp"

#include "util/can_rcv_buffer.h"
#include "util/can_tx_buffer.h"

// Tx Buffer Test

class can_tx_buffer_test : rockettest_test {
	static std::queue<can_msg_t> ref_buffer;
	static bool test_passed;

public:
	can_tx_buffer_test() : rockettest_test("can_tx_buffer_test") {}

	static bool can_tx_ready() {
		// Tx ready one in 4 chance
		return (rockettest_rand_field<int, 0x3>() == 0);
	}

	static void can_send(const can_msg_t *msg) {
		rockettest_check_expr_true(!ref_buffer.empty());

		rockettest_check_expr_true(msg->sid == ref_buffer.front().sid);
		rockettest_check_expr_true(msg->data_len == ref_buffer.front().data_len);
		for (std::uint8_t i = 0; i < msg->data_len; i++) {
			rockettest_check_expr_true(msg->data[i] == ref_buffer.front().data[i]);
		}

		ref_buffer.pop();
	}

	bool run_test() override {
		test_passed = true;

		void *pool = std::malloc(1000);

		txb_init(pool, 1000, can_send, can_tx_ready);

		for (int cycle = 0; cycle < 1000; cycle++) {
			// Push a message one in 8 chance
			if (rockettest_rand_field<int, 0x7>() == 0) {
				// Generate a random can message
				can_msg_t msg;
				msg.sid = rockettest_rand_field<can_sid_t, 0x1fffffff>();
				msg.data_len = rockettest_rand_field<uint8_t, 0xf>(); // 0 to 15
				if (msg.data_len > 8) {
					msg.data_len = 8;
				}
				for (std::size_t j = 0; j < 8; j++) {
					msg.data[j] = rockettest_rand_field<std::uint8_t, 0xff>();
				}

				// Push the message to both canlib tx queue and reference queue
				if (txb_enqueue(&msg)) {
					ref_buffer.push(msg);
				}
			}

			txb_heartbeat();
		}

		std::free(pool);

		return test_passed;
	}
};

bool can_tx_buffer_test::test_passed;
std::queue<can_msg_t> can_tx_buffer_test::ref_buffer;

can_tx_buffer_test can_tx_buffer_test_inst;

// Rcv Buffer Test

class can_rcv_buffer_test : rockettest_test {
	static std::queue<can_msg_t> ref_buffer;
	static bool test_passed;

public:
	can_rcv_buffer_test() : rockettest_test("can_rcv_buffer_test") {}

	static bool can_tx_ready() {
		// Tx ready one in 4 chance
		return (rockettest_rand_field<int, 0x3>() == 0);
	}

	static void can_send(const can_msg_t *msg) {
		rockettest_check_expr_true(!ref_buffer.empty());

		rockettest_check_expr_true(msg->sid == ref_buffer.front().sid);
		rockettest_check_expr_true(msg->data_len == ref_buffer.front().data_len);
		for (std::uint8_t i = 0; i < msg->data_len; i++) {
			rockettest_check_expr_true(msg->data[i] == ref_buffer.front().data[i]);
		}

		ref_buffer.pop();
	}

	bool run_test() override {
		test_passed = true;

		void *pool = std::malloc(1000);

		rcvb_init(pool, 1000);

		rockettest_check_expr_true(rcvb_is_empty());

		for (int cycle = 0; cycle < 1000; cycle++) {
			// Push a message 3 in 4 chance
			if (rockettest_rand_field<int, 0x3>() != 0) {
				// Generate a random can message
				can_msg_t msg;
				msg.sid = rockettest_rand_field<can_sid_t, 0x1fffffff>();
				msg.data_len = rockettest_rand_field<uint8_t, 0xf>(); // 0 to 15
				if (msg.data_len > 8) {
					msg.data_len = 8;
				}
				for (std::size_t j = 0; j < 8; j++) {
					msg.data[j] = rockettest_rand_field<std::uint8_t, 0xff>();
				}

				// Push the message to both canlib tx queue and reference queue
				rcvb_push_message(&msg);
				if (!rcvb_has_overflowed()) {
					ref_buffer.push(msg);
				} else {
					rockettest_check_expr_true(rcvb_is_full());
					rcvb_clear_overflow_flag();
				}
			}

			// Pop a message 1 in 2 chance
			if (rockettest_rand_field<int, 0x1>() == 0) {
				can_msg_t msg;
				if (rcvb_pop_message(&msg)) {
					rockettest_check_expr_true(msg.sid == ref_buffer.front().sid);
					rockettest_check_expr_true(msg.data_len == ref_buffer.front().data_len);
					for (std::uint8_t i = 0; i < msg.data_len; i++) {
						rockettest_check_expr_true(msg.data[i] == ref_buffer.front().data[i]);
					}

					ref_buffer.pop();
				}
			}

			// Peek in every cycle
			{
				can_msg_t msg;
				if (rcvb_peek_message(&msg)) {
					rockettest_check_expr_true(msg.sid == ref_buffer.front().sid);
					rockettest_check_expr_true(msg.data_len == ref_buffer.front().data_len);
					for (std::uint8_t i = 0; i < msg.data_len; i++) {
						rockettest_check_expr_true(msg.data[i] == ref_buffer.front().data[i]);
					}
				}
			}

			rockettest_check_expr_true(rcvb_is_empty() == ref_buffer.empty());
		}

		std::free(pool);

		return test_passed;
	}
};

bool can_rcv_buffer_test::test_passed;
std::queue<can_msg_t> can_rcv_buffer_test::ref_buffer;

can_rcv_buffer_test can_rcv_buffer_test_inst;
