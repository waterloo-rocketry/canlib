#include <cstdint>

#include "message_types.h"
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

		write_timestamp(timestamp_before, &msg);

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

class sid_extract_test : rockettest_test {
public:
	sid_extract_test() : rockettest_test("sid_extract_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;
		msg.sid = rockettest_rand<can_sid_t, 0x1fffffff>();

		can_msg_type_t msg_type_extracted;
		uint8_t board_type_id_extracted;
		uint8_t board_inst_id_extracted;
		uint8_t metadata_extracted;

		msg_type_extracted = static_cast<can_msg_type_t>((msg.sid >> 20) & 0x7f);
		board_type_id_extracted = (msg.sid >> 14) & 0x3f;
		board_inst_id_extracted = (msg.sid >> 8) & 0x3f;
		metadata_extracted = msg.sid & 0xff;

		can_msg_type_t msg_type_after;
		uint8_t board_type_id_after;
		uint8_t board_inst_id_after;
		uint8_t metadata_after;

		msg_type_after = get_message_type(&msg);
		board_type_id_after = get_board_type_unique_id(&msg);
		board_inst_id_after = get_board_inst_unique_id(&msg);
		metadata_after = get_message_metadata(&msg);

		rockettest_assert(msg_type_after == msg_type_extracted);
		rockettest_assert(board_type_id_after == board_type_id_extracted);
		rockettest_assert(board_inst_id_after == board_inst_id_extracted);
		rockettest_assert(metadata_after == metadata_extracted);

		return test_passed;
	}
};

sid_extract_test sid_extract_test_inst;
