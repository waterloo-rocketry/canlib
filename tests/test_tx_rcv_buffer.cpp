#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <queue>

#include "can.h"
#include "rockettest.hpp"

#include "util/can_tx_buffer.h"

class can_tx_buffer_test : rockettest_test {
	static std::queue<can_msg_t> ref_buffer;
	static bool test_passed;

public:
	can_tx_buffer_test() : rockettest_test("can_tx_buffer_test") {}

	static bool can_tx_ready() {
		// Tx ready one in 4 chance
		return (rockettest_rand<int, 0x3>() == 0);
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
			if (rockettest_rand<int, 0x7>() == 0) {
				// Generate a random can message
				can_msg_t msg;
				msg.sid = rockettest_rand<can_sid_t, 0x1fffffff>();
				msg.data_len = rockettest_rand<uint8_t, 0xf>(); // 0 to 15
				if (msg.data_len > 8) {
					msg.data_len = 8;
				}
				for (std::size_t j = 0; j < 8; j++) {
					msg.data[j] = rockettest_rand<std::uint8_t, 0xff>();
				}

				// Push the message to both canlib tx queue and reference queue
				if (txb_enqueue(&msg)) {
					ref_buffer.push(msg);
				}
			}

			txb_heartbeat();
		}

		return test_passed;
	}
};

bool can_tx_buffer_test::test_passed;
std::queue<can_msg_t> can_tx_buffer_test::ref_buffer;

can_tx_buffer_test can_tx_buffer_test_inst;
