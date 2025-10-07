#include <cstdint>

#include "rockettest.hpp"

#include "can.h"
#include "message/msg_common.h"

class timestamp_test : rockettest_test {
public:
	timestamp_test() : rockettest_test("timestamp_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;
		msg.data_len = 8;

		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();

		write_timestamp_2bytes(timestamp_before, &msg);

		std::uint16_t timestamp_extracted;

		timestamp_extracted = (static_cast<std::uint16_t>(msg.data[0]) << 8) | msg.data[1];

		rockettest_assert(timestamp_extracted == timestamp_before);

		std::uint16_t timestamp_after;

		timestamp_after = get_timestamp(&msg);

		rockettest_assert(timestamp_after == timestamp_before);

		return test_passed;
	}
};

timestamp_test timestamp_test_inst;
