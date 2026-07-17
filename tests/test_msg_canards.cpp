#include <cstdint>

#include "rockettest.hpp"

#include "can.h"
#include "message/msg_canards.h"
#include "message/msg_common.h"
#include "message_types.h"

class canard_firmware_error_message_test : rockettest_test {
public:
	canard_firmware_error_message_test() : rockettest_test("canard_firmware_error_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		can_msg_prio_t prio_before = rockettest_rand_field<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp_before = rockettest_rand_field<std::uint16_t>();
		can_canards_module_id_t module_id_before =
			rockettest_rand_field<can_canards_module_id_t, 0xff>();
		std::uint32_t error_bitfield_before = rockettest_rand_field<std::uint32_t>();
		can_canards_health_severity_t severity_before =
			rockettest_rand_field<can_canards_health_severity_t, 0xff>();

		build_canard_firmware_error_msg(prio_before,
										timestamp_before,
										module_id_before,
										error_bitfield_before,
										severity_before,
										&msg);

		can_canards_module_id_t module_id_extracted;
		std::uint32_t error_bitfield_extracted;
		can_canards_health_severity_t severity_extracted;

		module_id_extracted = (can_canards_module_id_t) static_cast<std::uint8_t>(msg.sid & 0xff);
		error_bitfield_extracted = (static_cast<std::uint32_t>(msg.data[2]) << 24) |
								   (static_cast<std::uint32_t>(msg.data[3]) << 16) |
								   (static_cast<std::uint32_t>(msg.data[4]) << 8) |
								   (static_cast<std::uint32_t>(msg.data[5]) << 0);
		severity_extracted = (can_canards_health_severity_t)msg.data[6];

		rockettest_check_expr_true(msg.data_len == 7);
		rockettest_check_expr_true(module_id_extracted == module_id_before);
		rockettest_check_expr_true(error_bitfield_extracted == error_bitfield_before);
		rockettest_check_expr_true(severity_extracted == severity_before);

		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_canards_module_id_t module_id_after;
		std::uint32_t error_bitfield_after;
		can_canards_health_severity_t severity_after;

		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		rockettest_check_expr_true(
			get_canard_firmware_error_msg(
				&msg, &module_id_after, &error_bitfield_after, &severity_after) == W_SUCCESS);

		rockettest_check_expr_true(type_after == MSG_CANARD_FIRMWARE_ERROR);
		rockettest_check_expr_true(timestamp_after == timestamp_before);
		rockettest_check_expr_true(module_id_after == module_id_before);
		rockettest_check_expr_true(error_bitfield_after == error_bitfield_before);
		rockettest_check_expr_true(severity_after == severity_before);

		can_msg_t invalid_type_msg = msg;
		invalid_type_msg.sid = build_sid(prio_before, MSG_SENSOR_ANALOG32, 0);
		rockettest_check_expr_true(
			get_canard_firmware_error_msg(
				&invalid_type_msg, &module_id_after, &error_bitfield_after, &severity_after) ==
			W_INVALID_PARAM);

		can_msg_t invalid_len_msg = msg;
		invalid_len_msg.data_len = 3;
		rockettest_check_expr_true(
			get_canard_firmware_error_msg(
				&invalid_len_msg, &module_id_after, &error_bitfield_after, &severity_after) ==
			W_DATA_FORMAT_ERROR);

		return test_passed;
	}
};

canard_firmware_error_message_test canard_firmware_error_message_test_inst;
