#include <cstddef>
#include <cstdint>

#include "common.h"
#include "rockettest.hpp"

#include "can.h"
#include "message/msg_actuator.h"
#include "message/msg_common.h"
#include "message_types.h"

class actuator_cmd_message_test : rockettest_test {
public:
	actuator_cmd_message_test() : rockettest_test("actuator_cmd_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		can_actuator_id_t actuator_id_before = rockettest_rand_field<can_actuator_id_t, 0xff>();
		can_actuator_state_t actuator_state_before =
			rockettest_rand_field<can_actuator_state_t, 0xff>();

		build_actuator_cmd_msg(
			prio_before, timestamp_before, actuator_id_before, actuator_state_before, &msg);

		can_actuator_id_t actuator_id_extracted;
		can_actuator_state_t actuator_state_extracted;

		actuator_id_extracted = static_cast<can_actuator_id_t>(msg.sid & 0xff);
		actuator_state_extracted = static_cast<can_actuator_state_t>(msg.data[2]);

		rockettest_check_expr_true(msg.data_len == 3);
		rockettest_check_expr_true(actuator_id_extracted == actuator_id_before);
		rockettest_check_expr_true(actuator_state_extracted == actuator_state_before);

		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_actuator_id_t actuator_id_after;
		can_actuator_state_t actuator_state_after;

		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		rockettest_check_expr_true(get_actuator_id(&msg, &actuator_id_after) == W_SUCCESS);
		rockettest_check_expr_true(get_cmd_actuator_state(&msg, &actuator_state_after) ==
								   W_SUCCESS);

		rockettest_check_expr_true(type_after == MSG_ACTUATOR_CMD);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(actuator_id_after == actuator_id_before);
		rockettest_check_expr_true(actuator_state_after == actuator_state_before);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 2;

		w_status_t parse_status = get_actuator_id(&invalid_len_msg, &actuator_id_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);
		parse_status = get_cmd_actuator_state(&invalid_len_msg, &actuator_state_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

actuator_cmd_message_test actuator_cmd_message_test_inst;

class actuator_status_message_test : rockettest_test {
public:
	actuator_status_message_test() : rockettest_test("actuator_status_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		can_actuator_id_t actuator_id_before = rockettest_rand_field<can_actuator_id_t, 0xff>();
		can_actuator_state_t actuator_curr_state_before =
			rockettest_rand_field<can_actuator_state_t, 0xff>();
		can_actuator_state_t actuator_cmd_state_before =
			rockettest_rand_field<can_actuator_state_t, 0xff>();

		build_actuator_status_msg(prio_before,
								  timestamp_before,
								  actuator_id_before,
								  actuator_cmd_state_before,
								  actuator_curr_state_before,
								  &msg);

		can_actuator_id_t actuator_id_extracted;
		can_actuator_state_t actuator_curr_state_extracted;
		can_actuator_state_t actuator_cmd_state_extracted;

		actuator_id_extracted = static_cast<can_actuator_id_t>(msg.sid & 0xff);
		actuator_cmd_state_extracted = static_cast<can_actuator_state_t>(msg.data[2]);
		actuator_curr_state_extracted = static_cast<can_actuator_state_t>(msg.data[3]);

		rockettest_check_expr_true(msg.data_len == 4);
		rockettest_check_expr_true(actuator_id_extracted == actuator_id_before);
		rockettest_check_expr_true(actuator_curr_state_extracted == actuator_curr_state_before);
		rockettest_check_expr_true(actuator_cmd_state_extracted == actuator_cmd_state_before);

		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_actuator_id_t actuator_id_after;
		can_actuator_state_t actuator_curr_state_after;
		can_actuator_state_t actuator_cmd_state_after;

		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		rockettest_check_expr_true(get_actuator_id(&msg, &actuator_id_after) == W_SUCCESS);
		rockettest_check_expr_true(get_curr_actuator_state(&msg, &actuator_curr_state_after) ==
								   W_SUCCESS);
		rockettest_check_expr_true(get_cmd_actuator_state(&msg, &actuator_cmd_state_after) ==
								   W_SUCCESS);

		rockettest_check_expr_true(type_after == MSG_ACTUATOR_STATUS);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(actuator_id_after == actuator_id_before);
		rockettest_check_expr_true(actuator_curr_state_after == actuator_curr_state_before);
		rockettest_check_expr_true(actuator_cmd_state_after == actuator_cmd_state_before);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 3;

		w_status_t parse_status = get_actuator_id(&invalid_len_msg, &actuator_id_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);
		parse_status = get_curr_actuator_state(&invalid_len_msg, &actuator_curr_state_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);
		parse_status = get_cmd_actuator_state(&invalid_len_msg, &actuator_curr_state_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio_before, MSG_ACTUATOR_CMD, actuator_id_before);
		parse_status = get_curr_actuator_state(&invalid_type_msg, &actuator_curr_state_after);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);

		return test_passed;
	}
};

actuator_status_message_test actuator_status_message_test_inst;

class actuator_message_general_test : rockettest_test {
public:
	actuator_message_general_test() : rockettest_test("actuator_message_general_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		can_actuator_id_t actuator_id_before = rockettest_rand_field<can_actuator_id_t, 0xff>();
		can_actuator_state_t actuator_state_before =
			rockettest_rand_field<can_actuator_state_t, 0xff>();

		build_actuator_cmd_msg(
			prio_before, timestamp_before, actuator_id_before, actuator_state_before, &msg);

		can_actuator_id_t actuator_id_after;
		can_actuator_state_t actuator_state_after;

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio_before, MSG_GENERAL_BOARD_STATUS, actuator_id_before);

		w_status_t parse_status = get_actuator_id(&invalid_type_msg, &actuator_id_after);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);

		parse_status = get_cmd_actuator_state(&invalid_type_msg, &actuator_state_after);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);

		return test_passed;
	}
};

actuator_message_general_test actuator_message_general_test_inst;
