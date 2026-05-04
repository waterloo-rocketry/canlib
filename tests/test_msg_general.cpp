#include <cstdint>

#include "common.h"
#include "rockettest.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_general.h"
#include "message_types.h"

class general_status_message_test : rockettest_test {
public:
	general_status_message_test() : rockettest_test("general_status_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		std::uint32_t board_error_bitfield_before = rockettest_rand_field<std::uint32_t>();
		build_general_board_status_msg(
			prio_before, timestamp_before, board_error_bitfield_before, &msg);

		std::uint16_t timestamp_extracted;
		std::uint32_t board_error_bitfield_extracted;

		timestamp_extracted = (static_cast<std::uint16_t>(msg.data[0]) << 8) | msg.data[1];
		board_error_bitfield_extracted = (static_cast<std::uint32_t>(msg.data[2]) << 24) |
										 (static_cast<std::uint32_t>(msg.data[3]) << 16) |
										 (static_cast<std::uint32_t>(msg.data[4]) << 8) |
										 (static_cast<std::uint32_t>(msg.data[5]) << 0);

		rockettest_check_expr_true(msg.data_len == 6);
		rockettest_check_expr_true(timestamp_extracted == timestamp_before);
		rockettest_check_expr_true(board_error_bitfield_extracted == board_error_bitfield_before);

		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		std::uint32_t board_error_bitfield_after;

		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		rockettest_check_expr_true(get_general_board_status(&msg, &board_error_bitfield_after) ==
								   W_SUCCESS);

		rockettest_check_expr_true(type_after == MSG_GENERAL_BOARD_STATUS);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(board_error_bitfield_after == board_error_bitfield_before);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio_before, MSG_RESET_CMD, 0);
		rockettest_check_expr_true(
			get_general_board_status(&invalid_type_msg, &board_error_bitfield_after) ==
			W_INVALID_PARAM);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 5;
		rockettest_check_expr_true(
			get_general_board_status(&invalid_len_msg, &board_error_bitfield_after) ==
			W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

general_status_message_test general_status_message_test_inst;

class reset_message_test : rockettest_test {
public:
	reset_message_test() : rockettest_test("reset_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		// Test build and parse

		can_msg_t msg;

		can_msg_prio_t prio = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		std::uint8_t board_type_id_before = rockettest_rand_field<can_board_type_id_t, 0xff>();
		std::uint8_t board_inst_id_before = rockettest_rand_field<can_board_inst_id_t, 0xff>();
		build_reset_msg(prio, timestamp_before, board_type_id_before, board_inst_id_before, &msg);

		std::uint16_t timestamp_extracted;
		std::uint8_t board_type_id_extracted;
		std::uint8_t board_inst_id_extracted;

		timestamp_extracted = (static_cast<std::uint16_t>(msg.data[0]) << 8) | msg.data[1];
		board_type_id_extracted = msg.data[2];
		board_inst_id_extracted = msg.data[3];

		rockettest_check_expr_true(msg.data_len == 4);
		rockettest_check_expr_true(timestamp_extracted == timestamp_before);
		rockettest_check_expr_true(board_type_id_extracted == board_type_id_before);
		rockettest_check_expr_true(board_inst_id_extracted == board_inst_id_before);

		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		std::uint8_t board_type_id_after;
		std::uint8_t board_inst_id_after;

		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		w_status_t parse_status =
			get_reset_board_id(&msg, &board_type_id_after, &board_inst_id_after);

		rockettest_check_expr_true(parse_status == W_SUCCESS);
		rockettest_check_expr_true(type_after == MSG_RESET_CMD);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(board_type_id_after == board_type_id_before);
		rockettest_check_expr_true(board_inst_id_after == board_inst_id_before);

		// invalid_type_msg and invalid_len_msg also used to test check_board_need_reset() below
		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio, MSG_GENERAL_BOARD_STATUS, 0);
		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 3;

		parse_status =
			get_reset_board_id(&invalid_type_msg, &board_type_id_after, &board_inst_id_after);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);
		parse_status =
			get_reset_board_id(&invalid_len_msg, &board_type_id_after, &board_inst_id_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);

		// Test check_board_need_reset()
		// Tests are compiled with BOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_ARMING and
		// BOARD_INST_UNIQUE_ID=BOARD_INST_ID_ROCKET

		bool board_need_reset;

		// Type any
		build_reset_msg(prio, timestamp_before, BOARD_TYPE_ID_ANY, board_inst_id_before, &msg);
		parse_status = check_board_need_reset(&msg, &board_need_reset);
		rockettest_check_expr_true(parse_status == W_SUCCESS);
		rockettest_check_expr_true(board_need_reset);

		// Type matching, inst any
		build_reset_msg(prio, timestamp_before, BOARD_TYPE_UNIQUE_ID, BOARD_INST_ID_ANY, &msg);
		parse_status = check_board_need_reset(&msg, &board_need_reset);
		rockettest_check_expr_true(parse_status == W_SUCCESS);
		rockettest_check_expr_true(board_need_reset);

		// Type matching, inst matching
		build_reset_msg(prio, timestamp_before, BOARD_TYPE_UNIQUE_ID, BOARD_INST_UNIQUE_ID, &msg);
		parse_status = check_board_need_reset(&msg, &board_need_reset);
		rockettest_check_expr_true(parse_status == W_SUCCESS);
		rockettest_check_expr_true(board_need_reset);

		// Type matching, inst not matching
		build_reset_msg(prio, timestamp_before, BOARD_TYPE_UNIQUE_ID, BOARD_INST_ID_GROUND, &msg);
		parse_status = check_board_need_reset(&msg, &board_need_reset);
		rockettest_check_expr_true(parse_status == W_SUCCESS);
		rockettest_check_expr_true(!board_need_reset);

		// Type not matching
		build_reset_msg(
			prio, timestamp_before, BOARD_TYPE_ID_ALTIMETER, BOARD_INST_UNIQUE_ID, &msg);
		parse_status = check_board_need_reset(&msg, &board_need_reset);
		rockettest_check_expr_true(parse_status == W_SUCCESS);
		rockettest_check_expr_true(!board_need_reset);

		parse_status = check_board_need_reset(&invalid_type_msg, &board_need_reset);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);
		parse_status = check_board_need_reset(&invalid_len_msg, &board_need_reset);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

reset_message_test reset_message_test_inst;

class debug_raw_message_test : rockettest_test {
public:
	debug_raw_message_test() : rockettest_test("debug_raw_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		std::uint8_t data_before[6];
		for (int i = 0; i < 6; ++i) {
			data_before[i] = rockettest_rand_field<std::uint8_t>();
		}
		build_debug_raw_msg(prio, timestamp_before, data_before, &msg);

		std::uint16_t timestamp_extracted;
		std::uint8_t data_extracted_byte;

		timestamp_extracted = (static_cast<std::uint16_t>(msg.data[0]) << 8) | msg.data[1];
		rockettest_check_expr_true(timestamp_extracted == timestamp_before);
		for (int i = 0; i < 6; ++i) {
			data_extracted_byte = msg.data[i + 2];
			rockettest_check_expr_true(data_extracted_byte == data_before[i]);
		}
		rockettest_check_expr_true(msg.data_len == 8);

		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		std::uint8_t data_after[8];

		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		w_status_t parse_status = get_debug_raw_data(&msg, data_after);
		rockettest_check_expr_true(parse_status == W_SUCCESS);

		rockettest_check_expr_true(type_after == MSG_DEBUG_RAW)
			rockettest_check_expr_true(timestamp_after == timestamp_before);
		for (int i = 0; i < 6; ++i) {
			rockettest_check_expr_true(data_after[i] == data_before[i]);
		}

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio, MSG_GENERAL_BOARD_STATUS, 0);
		parse_status = get_debug_raw_data(&invalid_type_msg, data_after);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 7;
		parse_status = get_debug_raw_data(&invalid_len_msg, data_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

debug_raw_message_test debug_raw_message_test_inst;

class config_set_message_test : rockettest_test {
public:
	config_set_message_test() : rockettest_test("config_set_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		std::uint8_t board_type_id_before = rockettest_rand_field<std::uint8_t, 0xff>();
		std::uint8_t board_inst_id_before = rockettest_rand_field<std::uint8_t, 0xff>();
		std::uint16_t config_id_before = rockettest_rand_field<std::uint16_t>();
		std::uint16_t config_value_before = rockettest_rand_field<std::uint16_t>();
		build_config_set_msg(prio,
							 timestamp_before,
							 board_type_id_before,
							 board_inst_id_before,
							 config_id_before,
							 config_value_before,
							 &msg);

		std::uint16_t timestamp_extracted;
		std::uint8_t board_type_id_extracted;
		std::uint8_t board_inst_id_extracted;
		std::uint16_t config_id_extracted;
		std::uint16_t config_value_extracted;

		timestamp_extracted = (static_cast<std::uint16_t>(msg.data[0]) << 8) | msg.data[1];
		board_type_id_extracted = msg.data[2];
		board_inst_id_extracted = msg.data[3];
		config_id_extracted = (static_cast<std::uint16_t>(msg.data[4]) << 8) | msg.data[5];
		config_value_extracted = (static_cast<std::uint16_t>(msg.data[6]) << 8) | msg.data[7];

		rockettest_check_expr_true(msg.data_len == 8);
		rockettest_check_expr_true(timestamp_extracted == timestamp_before);
		rockettest_check_expr_true(board_type_id_extracted == board_type_id_before);
		rockettest_check_expr_true(board_inst_id_extracted == board_inst_id_before);
		rockettest_check_expr_true(config_id_extracted == config_id_before);
		rockettest_check_expr_true(config_value_extracted == config_value_before);

		can_msg_type_t type_after;
		std::uint8_t board_type_id_after;
		std::uint8_t board_inst_id_after;
		std::uint16_t config_id_after;
		std::uint16_t config_value_after;

		type_after = get_message_type(&msg);
		w_status_t parse_status =
			get_config_set_target_board(&msg, &board_type_id_after, &board_inst_id_after);
		rockettest_check_expr_true(parse_status == W_SUCCESS);
		parse_status = get_config_id_value(&msg, &config_id_after, &config_value_after);
		rockettest_check_expr_true(parse_status == W_SUCCESS);

		rockettest_check_expr_true(type_after == MSG_CONFIG_SET);
		rockettest_check_expr_true(board_type_id_after == board_type_id_before);
		rockettest_check_expr_true(board_inst_id_after == board_inst_id_before);
		rockettest_check_expr_true(config_id_after == config_id_before);
		rockettest_check_expr_true(config_value_after == config_value_before);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio, MSG_CONFIG_STATUS, 0);
		parse_status = get_config_set_target_board(
			&invalid_type_msg, &board_type_id_after, &board_inst_id_after);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 7;
		parse_status = get_config_set_target_board(
			&invalid_len_msg, &board_type_id_after, &board_inst_id_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);
		parse_status = get_config_id_value(&invalid_len_msg, &config_id_after, &config_value_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

config_set_message_test config_set_message_test_inst;

class config_status_message_test : rockettest_test {
public:
	config_status_message_test() : rockettest_test("config_status_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		std::uint16_t config_id_before = rockettest_rand_field<std::uint16_t>();
		std::uint16_t config_value_before = rockettest_rand_field<std::uint16_t>();
		build_config_status_msg(
			prio, timestamp_before, config_id_before, config_value_before, &msg);

		std::uint16_t timestamp_extracted;
		std::uint16_t config_id_extracted;
		std::uint16_t config_value_extracted;

		timestamp_extracted = (static_cast<uint16_t>(msg.data[0]) << 8) | msg.data[1];
		config_id_extracted = (static_cast<uint16_t>(msg.data[2]) << 8) | msg.data[3];
		config_value_extracted = (static_cast<uint16_t>(msg.data[4]) << 8) | msg.data[5];

		rockettest_check_expr_true(msg.data_len == 6);
		rockettest_check_expr_true(timestamp_extracted == timestamp_before);
		rockettest_check_expr_true(config_id_extracted == config_id_before);
		rockettest_check_expr_true(config_value_extracted == config_value_before);

		can_msg_type_t type_after;
		std::uint16_t config_id_after;
		std::uint16_t config_value_after;

		type_after = get_message_type(&msg);
		w_status_t parse_status = get_config_id_value(&msg, &config_id_after, &config_value_after);
		rockettest_check_expr_true(parse_status == W_SUCCESS);

		rockettest_check_expr_true(type_after == MSG_CONFIG_STATUS);
		rockettest_check_expr_true(config_id_after == config_id_before);
		rockettest_check_expr_true(config_value_after == config_value_before);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 5;
		parse_status = get_config_id_value(&invalid_len_msg, &config_id_after, &config_value_after);
		rockettest_check_expr_true(parse_status == W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

config_status_message_test config_status_message_test_inst;

class config_message_general_test : rockettest_test {
public:
	config_message_general_test() : rockettest_test("config_message_general_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		std::uint16_t config_id_before = rockettest_rand_field<std::uint16_t>();
		std::uint16_t config_value_before = rockettest_rand_field<std::uint16_t>();
		build_config_status_msg(
			prio, timestamp_before, config_id_before, config_value_before, &msg);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio, MSG_GENERAL_BOARD_STATUS, 0);

		std::uint16_t config_id_after;
		std::uint16_t config_value_after;

		w_status_t parse_status =
			get_config_id_value(&invalid_type_msg, &config_id_after, &config_value_after);
		rockettest_check_expr_true(parse_status == W_INVALID_PARAM);

		return test_passed;
	}
};

config_message_general_test config_message_general_test_inst;
