#include <cstdint>

#include "rockettest.hpp"

#include "can.h"
#include "message/msg_actuator.h"
#include "message/msg_common.h"
#include "message_types.h"

/*
 * ACTUATOR_CMD (0x006) - Set actuator commanded state
 *
 * Byte 0-1: 2 byte timestamp
 * Byte 2:   ACTUATOR_ID
 * Byte 3:   ACTUATOR_CMD_STATE
 *
 * Tests: build_actuator_cmd_msg(), get_actuator_id(), get_cmd_actuator_state()
 */
class actuator_cmd_msg_test : rockettest_test {
public:
	actuator_cmd_msg_test() : rockettest_test("actuator_cmd_msg_test") {}

	bool run_test() override {
		bool test_passed = true;
		can_msg_t msg;

		// Fuzz test: Generate random values and verify encode/decode roundtrip
		// This catches issues with byte ordering, bit packing, and type conversions
		can_msg_prio_t prio = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp = rockettest_rand<std::uint16_t>();
		can_actuator_id_t actuator_id = rockettest_rand<can_actuator_id_t, ACTUATOR_ENUM_MAX - 1>();
		can_actuator_state_t actuator_cmd = rockettest_rand<can_actuator_state_t, ACT_STATE_ENUM_MAX - 1>();

		// Build message and verify all fields decode correctly
		rockettest_assert(build_actuator_cmd_msg(prio, timestamp, actuator_id, actuator_cmd, &msg) == true);
		rockettest_assert(msg.data_len == 4);  // CMD messages are 4 bytes: timestamp(2) + actuator_id(1) + cmd(1)
		rockettest_assert(get_message_type(&msg) == MSG_ACTUATOR_CMD);
		rockettest_assert(get_timestamp(&msg) == timestamp);
		rockettest_assert(get_actuator_id(&msg) == actuator_id);
		rockettest_assert(get_cmd_actuator_state(&msg) == actuator_cmd);

		// Boundary test: Maximum values for all fields
		// Ensures no overflow or truncation when using max values
		rockettest_assert(build_actuator_cmd_msg(PRIO_LOW, 0xFFFF, static_cast<can_actuator_id_t>(0xFF), ACT_STATE_ILLEGAL, &msg) == true);
		rockettest_assert(get_timestamp(&msg) == 0xFFFF);
		rockettest_assert(get_actuator_id(&msg) == 0xFF);
		rockettest_assert(get_cmd_actuator_state(&msg) == ACT_STATE_ILLEGAL);

		// Boundary test: Zero values for all fields
		// Ensures zero is handled correctly (common edge case)
		rockettest_assert(build_actuator_cmd_msg(PRIO_HIGHEST, 0, static_cast<can_actuator_id_t>(0), ACT_STATE_ON, &msg) == true);
		rockettest_assert(get_timestamp(&msg) == 0);
		rockettest_assert(get_actuator_id(&msg) == 0);
		rockettest_assert(get_cmd_actuator_state(&msg) == ACT_STATE_ON);

		// Safety test: Null pointer should return false, not crash
		rockettest_assert(build_actuator_cmd_msg(PRIO_MEDIUM, 0x1234, ACTUATOR_OX_INJECTOR_VALVE, ACT_STATE_ON, nullptr) == false);

		return test_passed;
	}
};

actuator_cmd_msg_test actuator_cmd_msg_test_inst;

/*
 * ACTUATOR_STATUS (0x008) - Actuator Status Message
 *
 * Byte 0-1: 2 byte timestamp
 * Byte 2:   ACTUATOR_ID
 * Byte 3:   ACTUATOR_CURR_STATE (current state from limit switches)
 * Byte 4:   ACTUATOR_CMD_STATE (commanded state)
 *
 * Tests: build_actuator_status_msg(), get_curr_actuator_state(), get_cmd_actuator_state()
 */
class actuator_status_msg_test : rockettest_test {
public:
	actuator_status_msg_test() : rockettest_test("actuator_status_msg_test") {}

	bool run_test() override {
		bool test_passed = true;
		can_msg_t msg;

		// Fuzz test: Generate random values for all status fields
		// Status messages have TWO state fields: current (from sensors) and commanded (desired)
		can_msg_prio_t prio = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint16_t timestamp = rockettest_rand<std::uint16_t>();
		can_actuator_id_t actuator_id = rockettest_rand<can_actuator_id_t, ACTUATOR_ENUM_MAX - 1>();
		can_actuator_state_t curr_state = rockettest_rand<can_actuator_state_t, ACT_STATE_ENUM_MAX - 1>();
		can_actuator_state_t cmd_state = rockettest_rand<can_actuator_state_t, ACT_STATE_ENUM_MAX - 1>();

		// Build message and verify all fields decode correctly
		rockettest_assert(build_actuator_status_msg(prio, timestamp, actuator_id, curr_state, cmd_state, &msg) == true);
		rockettest_assert(msg.data_len == 5);  // STATUS messages are 5 bytes: timestamp(2) + actuator_id(1) + curr_state(1) + cmd_state(1)
		rockettest_assert(get_message_type(&msg) == MSG_ACTUATOR_STATUS);
		rockettest_assert(get_timestamp(&msg) == timestamp);
		rockettest_assert(get_actuator_id(&msg) == actuator_id);
		rockettest_assert(get_curr_actuator_state(&msg) == curr_state);
		rockettest_assert(get_cmd_actuator_state(&msg) == cmd_state);

		// Functional test: Mismatched states (common real-world scenario)
		// Valve commanded OFF but sensor reads ON - indicates actuator hasn't responded yet
		rockettest_assert(build_actuator_status_msg(PRIO_MEDIUM, 0x5678, ACTUATOR_FUEL_INJECTOR_VALVE, ACT_STATE_ON, ACT_STATE_OFF, &msg) == true);
		rockettest_assert(get_curr_actuator_state(&msg) == ACT_STATE_ON);
		rockettest_assert(get_cmd_actuator_state(&msg) == ACT_STATE_OFF);

		// Boundary test: Maximum values for all fields
		rockettest_assert(build_actuator_status_msg(PRIO_LOW, 0xFFFF, static_cast<can_actuator_id_t>(0xFF), ACT_STATE_ILLEGAL, ACT_STATE_UNK, &msg) == true);
		rockettest_assert(get_timestamp(&msg) == 0xFFFF);
		rockettest_assert(get_actuator_id(&msg) == 0xFF);
		rockettest_assert(get_curr_actuator_state(&msg) == ACT_STATE_ILLEGAL);
		rockettest_assert(get_cmd_actuator_state(&msg) == ACT_STATE_UNK);

		// Safety test: Null pointer should return false
		rockettest_assert(build_actuator_status_msg(PRIO_HIGH, 0x1234, ACTUATOR_OX_INJECTOR_VALVE, ACT_STATE_ON, ACT_STATE_ON, nullptr) == false);

		// Type safety test: get_curr_actuator_state() only valid for STATUS messages
		// Should return -1 for wrong message type (CMD message here)
		can_msg_t wrong_msg;
		build_actuator_cmd_msg(PRIO_MEDIUM, 0x100, ACTUATOR_OX_INJECTOR_VALVE, ACT_STATE_ON, &wrong_msg);
		rockettest_assert(get_curr_actuator_state(&wrong_msg) == -1);

		return test_passed;
	}
};

actuator_status_msg_test actuator_status_msg_test_inst;

/*
 * ACTUATOR_ANALOG_CMD (0x007) - Analog Actuator Command
 *
 * Byte 0-1: 2 byte timestamp
 * Byte 2:   ACTUATOR_ID
 * Byte 3-4: ACTUATOR_ANALOG_CMD_STATE (16-bit analog, big-endian)
 *
 * Encoding: Big-endian (Byte 3 = MSB, Byte 4 = LSB)
 * Example: 0xABCD -> Byte 3 = 0xAB, Byte 4 = 0xCD
 *
 * Tests: build_actuator_analog_cmd_msg(), get_cmd_actuator_state_analog()
 */
class actuator_analog_cmd_msg_test : rockettest_test {
public:
	actuator_analog_cmd_msg_test() : rockettest_test("actuator_analog_cmd_msg_test") {}

	bool run_test() override {
		bool test_passed = true;
		can_msg_t msg;

		// Fuzz test: Generate random 16-bit analog command value
		// Analog commands are for continuous actuators (e.g., servo angle, PWM duty cycle)
		can_msg_prio_t prio = rockettest_rand<can_msg_prio_t, 0x3>();
		std::uint32_t timestamp = rockettest_rand<std::uint32_t>();
		can_actuator_id_t actuator_id = rockettest_rand<can_actuator_id_t, ACTUATOR_ENUM_MAX - 1>();
		std::uint16_t analog_cmd = rockettest_rand<std::uint16_t>();

		// Build message and verify all fields decode correctly
		rockettest_assert(build_actuator_analog_cmd_msg(prio, timestamp, actuator_id, analog_cmd, &msg) == true);
		rockettest_assert(msg.data_len == 5);  // ANALOG_CMD messages are 5 bytes: timestamp(2) + actuator_id(1) + analog_cmd(2)
		rockettest_assert(get_message_type(&msg) == MSG_ACTUATOR_ANALOG_CMD);
		rockettest_assert(get_timestamp(&msg) == static_cast<std::uint16_t>(timestamp));  // timestamp truncated to 16 bits
		rockettest_assert(get_actuator_id(&msg) == actuator_id);
		rockettest_assert(get_cmd_actuator_state_analog(&msg) == analog_cmd);

		// Boundary test: Minimum analog value (0x0000)
		rockettest_assert(build_actuator_analog_cmd_msg(PRIO_HIGHEST, 0, ACTUATOR_CANARD_ANGLE, 0x0000, &msg) == true);
		rockettest_assert(get_cmd_actuator_state_analog(&msg) == 0x0000);

		// Boundary test: Maximum analog value (0xFFFF = 65535)
		rockettest_assert(build_actuator_analog_cmd_msg(PRIO_LOW, 0xFFFFFFFF, static_cast<can_actuator_id_t>(0xFF), 0xFFFF, &msg) == true);
		rockettest_assert(get_cmd_actuator_state_analog(&msg) == 0xFFFF);

		// Encoding test: Verify big-endian byte order
		// 0xABCD should be stored as: data[3] = 0xAB (MSB), data[4] = 0xCD (LSB)
		rockettest_assert(build_actuator_analog_cmd_msg(PRIO_MEDIUM, 0x1234, ACTUATOR_CANARD_ANGLE, 0xABCD, &msg) == true);
		rockettest_assert(msg.data[3] == 0xAB);  // Most significant byte
		rockettest_assert(msg.data[4] == 0xCD);  // Least significant byte
		rockettest_assert(get_cmd_actuator_state_analog(&msg) == 0xABCD);

		// Safety test: Null pointer should return false
		rockettest_assert(build_actuator_analog_cmd_msg(PRIO_HIGH, 0x5678, ACTUATOR_CANARD_ANGLE, 0x1234, nullptr) == false);

		// Type safety test: get_cmd_actuator_state_analog() only valid for ANALOG_CMD messages
		// Should return 0 for wrong message type (digital CMD message here)
		can_msg_t wrong_msg;
		build_actuator_cmd_msg(PRIO_MEDIUM, 0x100, ACTUATOR_OX_INJECTOR_VALVE, ACT_STATE_ON, &wrong_msg);
		rockettest_assert(get_cmd_actuator_state_analog(&wrong_msg) == 0);

		return test_passed;
	}
};

actuator_analog_cmd_msg_test actuator_analog_cmd_msg_test_inst;

/*
 * Cross-function and integration tests
 * - Verify get_actuator_id() works across all 3 message types
 * - Test all priority levels (PRIO_LOW, PRIO_MEDIUM, PRIO_HIGH) encode correctly
 * - Test specific actuator IDs from enums (ACTUATOR_OX_INJECTOR_VALVE, etc.)
 * - Verify getters return proper error values for wrong message types
 */
class actuator_integration_test : rockettest_test {
public:
	actuator_integration_test() : rockettest_test("actuator_integration_test") {}

	bool run_test() override {
		bool test_passed = true;
		can_msg_t msg;

		// Cross-message test: Verify get_actuator_id() works for all 3 message types
		// This ensures the actuator_id field is in the same location (byte 2) for all types
		build_actuator_cmd_msg(PRIO_MEDIUM, 0x100, ACTUATOR_OX_INJECTOR_VALVE, ACT_STATE_ON, &msg);
		rockettest_assert(get_actuator_id(&msg) == ACTUATOR_OX_INJECTOR_VALVE);

		build_actuator_analog_cmd_msg(PRIO_MEDIUM, 0x200, ACTUATOR_CANARD_ANGLE, 0x1234, &msg);
		rockettest_assert(get_actuator_id(&msg) == ACTUATOR_CANARD_ANGLE);

		build_actuator_status_msg(PRIO_MEDIUM, 0x300, ACTUATOR_FUEL_INJECTOR_VALVE, ACT_STATE_ON, ACT_STATE_OFF, &msg);
		rockettest_assert(get_actuator_id(&msg) == ACTUATOR_FUEL_INJECTOR_VALVE);

		// Priority encoding test: Verify all 4 priority levels encode correctly in SID
		// Priority is stored in bits 27-28 of the 29-bit CAN standard identifier (SID)
		build_actuator_cmd_msg(PRIO_HIGHEST, 0x100, ACTUATOR_OX_INJECTOR_VALVE, ACT_STATE_ON, &msg);
		rockettest_assert((msg.sid >> 27) == PRIO_HIGHEST);

		build_actuator_cmd_msg(PRIO_HIGH, 0x100, ACTUATOR_OX_INJECTOR_VALVE, ACT_STATE_ON, &msg);
		rockettest_assert((msg.sid >> 27) == PRIO_HIGH);

		build_actuator_cmd_msg(PRIO_MEDIUM, 0x100, ACTUATOR_OX_INJECTOR_VALVE, ACT_STATE_ON, &msg);
		rockettest_assert((msg.sid >> 27) == PRIO_MEDIUM);

		build_actuator_cmd_msg(PRIO_LOW, 0x100, ACTUATOR_OX_INJECTOR_VALVE, ACT_STATE_ON, &msg);
		rockettest_assert((msg.sid >> 27) == PRIO_LOW);

		// Enum test: Verify specific actuator IDs from the enum work correctly
		// Tests realistic actuator IDs used in the rocket system
		build_actuator_status_msg(PRIO_MEDIUM, 0x400, ACTUATOR_ROCKET_CHARGE_ENABLE, ACT_STATE_ON, ACT_STATE_ON, &msg);
		rockettest_assert(get_actuator_id(&msg) == ACTUATOR_ROCKET_CHARGE_ENABLE);

		build_actuator_analog_cmd_msg(PRIO_MEDIUM, 0x500, ACTUATOR_PAYLOAD_MOTOR_ENABLE, 0x5678, &msg);
		rockettest_assert(get_actuator_id(&msg) == ACTUATOR_PAYLOAD_MOTOR_ENABLE);

		// Error handling test: Null pointer should return -1
		rockettest_assert(get_actuator_id(nullptr) == -1);

		// Type safety test: All getters should return error values for wrong message types
		// Create a sensor message (non-actuator type) and verify all actuator getters fail gracefully
		msg.sid = SID(PRIO_MEDIUM, MSG_SENSOR_TEMP);
		rockettest_assert(get_actuator_id(&msg) == -1);                      // Not an actuator message
		rockettest_assert(get_curr_actuator_state(&msg) == -1);              // Not a STATUS message
		rockettest_assert(get_cmd_actuator_state(&msg) == -1);               // Not a CMD or STATUS message
		rockettest_assert(get_cmd_actuator_state_analog(&msg) == 0);         // Not an ANALOG_CMD message

		return test_passed;
	}
};

actuator_integration_test actuator_integration_test_inst;
