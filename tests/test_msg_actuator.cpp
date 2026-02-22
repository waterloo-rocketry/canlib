#include <cstdint>

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

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		can_actuator_id_t actuator_id_before = rockettest_rand<can_actuator_id_t, 0xff>();
		can_actuator_state_t actuator_state_before = rockettest_rand<can_actuator_state_t>();

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
		get_cmd_actuator_state(&msg);

		rockettest_check_expr_true(type_after == MSG_ACTUATOR_CMD);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(actuator_id_after == actuator_id_before);
		rockettest_check_expr_true(actuator_state_after == actuator_state_before);

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

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		can_actuator_id_t actuator_id_before = rockettest_rand<can_actuator_id_t, 0xff>();
		can_actuator_state_t actuator_curr_state_before = rockettest_rand<can_actuator_state_t>();
		can_actuator_state_t actuator_cmd_state_before = rockettest_rand<can_actuator_state_t>();

		build_actuator_status_msg(prio_before,
								  timestamp_before,
								  actuator_id_before,
								  actuator_curr_state_before,
								  actuator_cmd_state_before,
								  &msg);

		can_actuator_id_t actuator_id_extracted;
		can_actuator_state_t actuator_curr_state_extracted;
		can_actuator_state_t actuator_cmd_state_extracted;

		actuator_id_extracted = static_cast<can_actuator_id_t>(msg.sid & 0xff);
		actuator_curr_state_extracted = static_cast<can_actuator_state_t>(msg.data[2]);
		actuator_cmd_state_extracted = static_cast<can_actuator_state_t>(msg.data[3]);

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
		get_actuator_id(&msg);
		get_curr_actuator_state(&msg);
		get_cmd_actuator_state(&msg);

		rockettest_check_expr_true(type_after == MSG_ACTUATOR_STATUS);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(actuator_id_after == actuator_id_before);
		rockettest_check_expr_true(actuator_curr_state_after == actuator_curr_state_before);
		rockettest_check_expr_true(actuator_cmd_state_after == actuator_cmd_state_before);

		return test_passed;
	}
};

actuator_status_message_test actuator_status_message_test_inst;