#include <cstdint>

#include "rockettest.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_recovery.h"
#include "message_types.h"

class recovery_cmd_message_test : rockettest_test {
public:
	recovery_cmd_message_test() : rockettest_test("recovery_cmd_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		can_altimeter_id_t alt_id_before = rockettest_rand<can_altimeter_id_t, 0xff>();
		can_alt_arm_state_t alt_arm_state_before = rockettest_rand<can_alt_arm_state_t, 0xff>();

		build_alt_arm_cmd_msg(
			prio_before, timestamp_before, alt_id_before, alt_arm_state_before, &msg);

		can_altimeter_id_t alt_id_extracted;
		can_alt_arm_state_t alt_arm_state_extracted;

		alt_id_extracted = static_cast<can_altimeter_id_t>(msg.sid & 0xff);
		alt_arm_state_extracted = static_cast<can_alt_arm_state_t>(msg.data[2]);

		rockettest_check_expr_true(msg.data_len == 3);
		rockettest_check_expr_true(alt_id_extracted == alt_id_before);
		rockettest_check_expr_true(alt_arm_state_extracted == alt_arm_state_before);

		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_altimeter_id_t alt_id_after;
		can_alt_arm_state_t alt_arm_state_after;

		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_alt_arm_state(&msg, &alt_id_after, &alt_arm_state_after);

		rockettest_check_expr_true(type_after == MSG_ALT_ARM_CMD);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(alt_id_after == alt_id_before);
		rockettest_check_expr_true(alt_arm_state_after == alt_arm_state_before);

		return test_passed;
	}
};

recovery_cmd_message_test recovery_cmd_message_test_inst;

class recovery_status_message_test : rockettest_test {
public:
	recovery_status_message_test() : rockettest_test("recovery_status_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
		can_altimeter_id_t alt_id_before = rockettest_rand<can_altimeter_id_t, 0xff>();
		can_alt_arm_state_t alt_arm_state_before = rockettest_rand<can_alt_arm_state_t, 0xff>();
		std::uint16_t v_drogue_before = rockettest_rand<std::uint16_t>();
		std::uint16_t v_main_before = rockettest_rand<std::uint16_t>();

		build_alt_arm_status_msg(prio_before,
								 timestamp_before,
								 alt_id_before,
								 alt_arm_state_before,
								 v_drogue_before,
								 v_main_before,
								 &msg);

		can_altimeter_id_t alt_id_extracted;
		can_alt_arm_state_t alt_arm_state_extracted;
		uint16_t v_drogue_extracted;
		uint16_t v_main_extracted;

		alt_id_extracted = static_cast<can_altimeter_id_t>(msg.sid & 0xff);
		alt_arm_state_extracted = static_cast<can_alt_arm_state_t>(msg.data[2]);
		v_drogue_extracted = (static_cast<uint16_t>(msg.data[3]) << 8) | msg.data[4];
		v_main_extracted = (static_cast<uint16_t>(msg.data[5]) << 8) | msg.data[6];

		rockettest_check_expr_true(msg.data_len == 7);
		rockettest_check_expr_true(alt_id_extracted == alt_id_before);
		rockettest_check_expr_true(alt_arm_state_extracted == alt_arm_state_before);
		rockettest_check_expr_true(v_drogue_extracted == v_drogue_before);
		rockettest_check_expr_true(v_main_extracted == v_main_before);

		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_altimeter_id_t alt_id_after;
		can_alt_arm_state_t alt_arm_state_after;
		uint16_t v_drogue_after;
		uint16_t v_main_after;

		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);

		get_alt_arm_state(&msg, &alt_id_after, &alt_arm_state_after);
		get_pyro_voltage_data(&msg, &v_drogue_after, &v_main_after);

		rockettest_check_expr_true(type_after == MSG_ALT_ARM_STATUS);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(alt_id_after == alt_id_before);
		rockettest_check_expr_true(alt_arm_state_after == alt_arm_state_before);
		rockettest_check_expr_true(v_drogue_after == v_drogue_before);
		rockettest_check_expr_true(v_main_after == v_main_before);

		return test_passed;
	}
};

recovery_status_message_test recovery_status_message_test_inst;
