#include <cstdint>
#include <cstring>

#include "rockettest.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_stream.h"
#include "message_types.h"

namespace {

	constexpr std::uint32_t kMax24BitValue = (1u << 24) - 1;

}

class stream_status_msg_test : public rockettest_test {
public:
	stream_status_msg_test() : rockettest_test("stream_status_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg{};

		can_msg_prio_t prio = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp = rockettest_rand<std::uint16_t>();
		std::uint32_t total_size = rockettest_rand<std::uint32_t, kMax24BitValue>();
		std::uint32_t tx_size = rockettest_rand<std::uint32_t, kMax24BitValue>();

		bool build_result = build_stream_status_msg(prio, timestamp, total_size, tx_size, &msg);
		rockettest_assert(build_result);
		rockettest_assert(msg.sid == SID(prio, MSG_STREAM_STATUS));
		rockettest_assert(msg.data_len == 8);

		std::uint32_t total_size_out = 0;
		std::uint32_t tx_size_out = 0;
		bool parsed = get_stream_status(&msg, &total_size_out, &tx_size_out);
		rockettest_assert(parsed);
		rockettest_assert(total_size_out == total_size);
		rockettest_assert(tx_size_out == tx_size);

		bool oversized_total =
			build_stream_status_msg(prio, timestamp, kMax24BitValue + 1u, tx_size, &msg);
		rockettest_assert(oversized_total == false);

		bool oversized_tx =
			build_stream_status_msg(prio, timestamp, total_size, kMax24BitValue + 1u, &msg);
		rockettest_assert(oversized_tx == false);

		can_msg_t invalid_msg = msg;
		invalid_msg.sid = SID(prio, MSG_STREAM_DATA);
		bool invalid_type = get_stream_status(&invalid_msg, &total_size_out, &tx_size_out);
		rockettest_assert(invalid_type == false);

		return test_passed;
	}
};

stream_status_msg_test stream_status_msg_test_inst;

class stream_data_msg_test : public rockettest_test {
public:
	stream_data_msg_test() : rockettest_test("stream_data_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg{};

		can_msg_prio_t prio = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp = rockettest_rand<std::uint16_t>();
		std::uint8_t seq_id = rockettest_rand<std::uint8_t>();
		std::uint8_t payload_len = STREAM_DATA_MAX_PAYLOAD_LEN;
		std::uint8_t payload[STREAM_DATA_MAX_PAYLOAD_LEN];
		for (std::uint8_t i = 0; i < payload_len; ++i) {
			payload[i] = rockettest_rand<std::uint8_t>();
		}

		bool build_result =
			build_stream_data_msg(prio, timestamp, seq_id, payload, payload_len, &msg);
		rockettest_assert(build_result);
		rockettest_assert(msg.sid == SID(prio, MSG_STREAM_DATA));
		rockettest_assert(msg.data_len == static_cast<std::uint8_t>(payload_len + 3));
		rockettest_assert(msg.data[2] == seq_id);
		rockettest_assert(std::memcmp(&msg.data[3], payload, payload_len) == 0);

		std::uint8_t seq_id_out = 0;
		std::uint8_t payload_out[STREAM_DATA_MAX_PAYLOAD_LEN] = {};
		std::uint8_t payload_len_out = 0;
		bool parsed = get_stream_data(&msg, &seq_id_out, payload_out, &payload_len_out);
		rockettest_assert(parsed);
		rockettest_assert(seq_id_out == seq_id);
		rockettest_assert(payload_len_out == payload_len);
		rockettest_assert(std::memcmp(payload_out, payload, payload_len) == 0);

		std::uint8_t overflow_payload[STREAM_DATA_MAX_PAYLOAD_LEN + 1] = {};
		bool overflow_result = build_stream_data_msg(
			prio, timestamp, seq_id, overflow_payload, STREAM_DATA_MAX_PAYLOAD_LEN + 1, &msg);
		rockettest_assert(overflow_result == false);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.sid = SID(prio, MSG_STREAM_DATA);
		invalid_len_msg.data_len = 3;
		bool invalid_len =
			get_stream_data(&invalid_len_msg, &seq_id_out, payload_out, &payload_len_out);
		rockettest_assert(invalid_len == false);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = SID(prio, MSG_STREAM_RETRY);
		invalid_type_msg.data_len = msg.data_len;
		bool invalid_type =
			get_stream_data(&invalid_type_msg, &seq_id_out, payload_out, &payload_len_out);
		rockettest_assert(invalid_type == false);

		return test_passed;
	}
};

stream_data_msg_test stream_data_msg_test_inst;

class stream_retry_msg_test : public rockettest_test {
public:
	stream_retry_msg_test() : rockettest_test("stream_retry_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg{};

		can_msg_prio_t prio = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp = rockettest_rand<std::uint16_t>();
		std::uint8_t seq_id = rockettest_rand<std::uint8_t>();

		bool build_result = build_stream_retry_msg(prio, timestamp, seq_id, &msg);
		rockettest_assert(build_result);
		rockettest_assert(msg.sid == SID(prio, MSG_STREAM_RETRY));
		rockettest_assert(msg.data_len == 3);
		rockettest_assert(msg.data[2] == seq_id);

		std::uint8_t seq_id_out = 0;
		bool parsed = get_stream_retry_seq_id(&msg, &seq_id_out);
		rockettest_assert(parsed);
		rockettest_assert(seq_id_out == seq_id);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = SID(prio, MSG_STREAM_STATUS);
		bool invalid_type = get_stream_retry_seq_id(&invalid_type_msg, &seq_id_out);
		rockettest_assert(invalid_type == false);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 2;
		bool invalid_len = get_stream_retry_seq_id(&invalid_len_msg, &seq_id_out);
		rockettest_assert(invalid_len == false);

		return test_passed;
	}
};

stream_retry_msg_test stream_retry_msg_test_inst;
