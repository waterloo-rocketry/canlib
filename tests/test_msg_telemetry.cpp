#include <cstdint>

#include "rockettest.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_telemetry.h"
#include "message_types.h"

class telemetry_info_message_test : rockettest_test {
public:
	telemetry_info_message_test() : rockettest_test("telemetry_info_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		std::uint8_t channel_id_before = rockettest_rand_field<std::uint8_t>();
		std::uint8_t lqi_before = rockettest_rand_field<std::uint8_t>();
		std::int8_t rssi_before = rockettest_rand_field<std::int8_t>();

		build_telemetry_info_msg(
			prio_before, timestamp_before, channel_id_before, lqi_before, rssi_before, &msg);

		std::uint8_t channel_id_extracted;
		std::uint8_t lqi_extracted;
		std::int8_t rssi_extracted;

		channel_id_extracted = static_cast<std::uint8_t>(msg.sid & 0xff);
		lqi_extracted = msg.data[2];
		rssi_extracted = msg.data[3];

		rockettest_check_expr_true(msg.data_len == 4);
		rockettest_check_expr_true(channel_id_extracted == channel_id_before);
		rockettest_check_expr_true(lqi_extracted == lqi_before);
		rockettest_check_expr_true(rssi_extracted == rssi_before);

		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		std::uint8_t channel_id_after;
		std::uint8_t lqi_after;
		std::int8_t rssi_after;

		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		rockettest_check_expr_true(
			get_telemetry_info_msg(&msg, &channel_id_after, &lqi_after, &rssi_after) == W_SUCCESS);

		rockettest_check_expr_true(type_after == MSG_TELEMETRY_INFO);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(channel_id_after == channel_id_before);
		rockettest_check_expr_true(lqi_after == lqi_before);
		rockettest_check_expr_true(rssi_after == rssi_before);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio_before, MSG_SENSOR_ANALOG32, 0);
		rockettest_check_expr_true(
			get_telemetry_info_msg(&invalid_type_msg, &channel_id_after, &lqi_after, &rssi_after) ==
			W_INVALID_PARAM);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 3;
		rockettest_check_expr_true(
			get_telemetry_info_msg(&invalid_len_msg, &channel_id_after, &lqi_after, &rssi_after) ==
			W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

telemetry_info_message_test telemetry_info_message_test_inst;

class telemetry_state_switch_message_test : rockettest_test {
public:
	telemetry_state_switch_message_test()
		: rockettest_test("telemetry_state_switch_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		std::uint8_t channel_id_before = rockettest_rand_field<std::uint8_t>();

		build_telemetry_state_switch_msg(prio_before, timestamp_before, channel_id_before, &msg);

		std::uint8_t channel_id_extracted;

		channel_id_extracted = static_cast<std::uint8_t>(msg.sid & 0xff);

		rockettest_check_expr_true(msg.data_len == 2);
		rockettest_check_expr_true(channel_id_extracted == channel_id_before);

		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		std::uint8_t channel_id_after;

		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		rockettest_check_expr_true(get_telemetry_state_switch_msg(&msg, &channel_id_after) ==
								   W_SUCCESS);

		rockettest_check_expr_true(type_after == MSG_TELEMETRY_STATE_SWITCH);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(channel_id_after == channel_id_before);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio_before, MSG_SENSOR_ANALOG32, 0);
		rockettest_check_expr_true(get_telemetry_state_switch_msg(
									   &invalid_type_msg, &channel_id_after) == W_INVALID_PARAM);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 3;
		rockettest_check_expr_true(get_telemetry_state_switch_msg(
									   &invalid_len_msg, &channel_id_after) == W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

telemetry_state_switch_message_test telemetry_state_switch_message_test_inst;
