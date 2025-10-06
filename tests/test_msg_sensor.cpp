/*
 * Sensor Message Unit Tests
 *
 * Comprehensive tests for all CAN sensor message types.
 * Each test follows the pattern:
 * 1. Generate random input data
 * 2. Build CAN message
 * 3. Extract data from message
 * 4. Verify round-trip consistency
 */

#include <cstdint>

#include "rockettest.hpp"
#include "test_msg_sensor.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_sensor.h"
#include "message_types.h"

/*
 * Temperature Sensor Message Test (MSG_SENSOR_TEMP)
 *
 * Tests temperature measurement data transmission.
 * Temperature is stored as 24-bit signed value in units of 1/1024th degree C.
 * Range: approximately -8192°C to +8192°C with 0.001°C resolution.
 *
 * Message format:
 * - Priority: 2 bits (0-3)
 * - Timestamp: 16 bits (2 bytes) - milliseconds
 * - Sensor number: 8 bits (1 byte) - identifies which temperature sensor
 * - Temperature: 32 bits (4 bytes, only 24 bits used) - signed, in 1/1024°C units
 */
class temp_data_msg_test : rockettest_test {
public:
	temp_data_msg_test() : rockettest_test("temp_data_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		// Generate random test data
		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>(); // 0x3 = 2-bit mask (priority range 0-3)
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();   // Full 16-bit range
		std::uint8_t sensor_num_before = rockettest_rand<std::uint8_t>();    // Any sensor ID
		std::int32_t temp_before = rockettest_rand<std::int32_t>();          // Full 32-bit range (24 bits used)

		// Build message from test data
		build_temp_data_msg(prio_before, timestamp_before, sensor_num_before, temp_before, &msg);

		// Extract data from built message
		bool is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		std::uint8_t sensor_num_after;
		std::int32_t temp_after;

		is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_temp_data(&msg, &sensor_num_after, &temp_after);

		// Verify round-trip consistency: all data should match original values
		rockettest_assert(is_sensor_data_after == true);           // Message correctly identified as sensor data
		rockettest_assert(type_after == MSG_SENSOR_TEMP);          // Message type preserved
		rockettest_assert(timestamp_after == timestamp_before);    // Timestamp preserved
		rockettest_assert(sensor_num_after == sensor_num_before);  // Sensor ID preserved
		rockettest_assert(temp_after == temp_before);              // Temperature value preserved

		return test_passed;
	}
};

// Global instance auto-registers test with rockettest framework on program initialization
temp_data_msg_test temp_data_msg_test_inst;

/*
 * Altitude Sensor Message Test (MSG_SENSOR_ALTITUDE)
 *
 * Tests altitude data from altimeters with apogee detection state.
 * Altitude is signed to support both above and below launch site elevation.
 *
 * Message format:
 * - Priority: 2 bits (0-3)
 * - Timestamp: 16 bits (2 bytes) - milliseconds
 * - Altitude: 32 bits (4 bytes) - signed, in meters or feet (system-dependent)
 * - Apogee state: 8 bits (1 byte) - APOGEE_UNKNOWN, APOGEE_NOT_REACHED, or APOGEE_REACHED
 */
class altitude_data_msg_test : rockettest_test {
public:
	altitude_data_msg_test() : rockettest_test("altitude_data_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		// Generate random test data
		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();           // 0x3 = 2-bit mask (priority range 0-3)
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();             // Full 16-bit range
		std::int32_t altitude_before = rockettest_rand<std::int32_t>();                // Full 32-bit signed range
		can_apogee_state_t apogee_state_before = rockettest_rand<can_apogee_state_t, 0xff>(); // 0xff allows any enum value

		// Build message from test data
		build_altitude_data_msg(
			prio_before, timestamp_before, altitude_before, apogee_state_before, &msg);

		// Extract data from built message
		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		std::int32_t altitude_after;
		can_apogee_state_t apogee_state_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_altitude_data(&msg, &altitude_after, &apogee_state_after);

		// Verify round-trip consistency: all data should match original values
		rockettest_assert(msg_is_sensor_data_after == true);           // Message correctly identified as sensor data
		rockettest_assert(type_after == MSG_SENSOR_ALTITUDE);          // Message type preserved
		rockettest_assert(timestamp_after == timestamp_before);        // Timestamp preserved
		rockettest_assert(altitude_after == altitude_before);          // Altitude value preserved
		rockettest_assert(apogee_state_after == apogee_state_before);  // Apogee detection state preserved

		return test_passed;
	}
};

altitude_data_msg_test altitude_data_msg_test_inst;

/*
 * Analog Sensor Message Test (MSG_SENSOR_ANALOG)
 *
 * Tests generic analog sensor data (voltages, currents, pressures, etc.).
 * Sensor ID determines the specific sensor type and units for interpretation.
 * Examples: battery voltage, current draw, tank pressure, continuity checks.
 *
 * Message format:
 * - Priority: 2 bits (0-3)
 * - Timestamp: 16 bits (2 bytes) - milliseconds
 * - Sensor ID: 8 bits (1 byte) - identifies specific analog sensor and its units
 * - Sensor data: 16 bits (2 bytes) - raw ADC value or scaled reading (ID-dependent)
 */
class analog_sensor_message_test : rockettest_test {
public:
	analog_sensor_message_test() : rockettest_test("analog_sensor_message_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		// Generate random test data
		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();           // 0x3 = 2-bit mask (priority range 0-3)
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();             // Full 16-bit range
		can_analog_sensor_id_t sensor_id_before = rockettest_rand<can_analog_sensor_id_t, 0xff>(); // 0xff allows any sensor ID
		std::uint16_t sensor_data_before = rockettest_rand<std::uint16_t>();           // Full 16-bit range

		// Build message from test data
		build_analog_data_msg(
			prio_before, timestamp_before, sensor_id_before, sensor_data_before, &msg);

		// Extract data from built message
		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_analog_sensor_id_t sensor_id_after;
		std::uint16_t sensor_data_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_analog_data(&msg, &sensor_id_after, &sensor_data_after);

		// Verify round-trip consistency: all data should match original values
		rockettest_assert(msg_is_sensor_data_after == true);           // Message correctly identified as sensor data
		rockettest_assert(type_after == MSG_SENSOR_ANALOG);            // Message type preserved
		rockettest_assert(timestamp_after == timestamp_before);        // Timestamp preserved
		rockettest_assert(sensor_id_after == sensor_id_before);        // Sensor ID preserved
		rockettest_assert(sensor_data_after == sensor_data_before);    // Sensor data preserved

		return test_passed;
	}
};

analog_sensor_message_test analog_sensor_message_test_inst;

/*
 * IMU Data Message Test (MSG_SENSOR_IMU_X/Y/Z)
 *
 * Tests inertial measurement unit data (accelerometer + gyroscope combined).
 * IMUs send separate messages for X, Y, Z axes. This test uses X axis as
 * representative example (Y and Z follow identical format with different axis).
 *
 * Message format:
 * - Priority: 2 bits (0-3)
 * - Timestamp: 16 bits (2 bytes) - milliseconds
 * - Axis: char (X, Y, or Z) - encoded in message type (MSG_SENSOR_IMU_X/Y/Z)
 * - IMU ID: 8 bits (1 byte) - identifies which IMU (multiple IMUs supported)
 * - Linear acceleration: 16 bits (2 bytes) - in m/s² or g's (scaled)
 * - Angular velocity: 16 bits (2 bytes) - in rad/s or degrees/s (scaled)
 */
class imu_data_msg_test : rockettest_test {
public:
	imu_data_msg_test() : rockettest_test("imu_data_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		// Generate random test data
		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();           // 0x3 = 2-bit mask (priority range 0-3)
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();             // Full 16-bit range
		char axis_before = 'X';                                                        // Testing X axis (Y/Z use same pattern)
		can_imu_id_t imu_id_before = rockettest_rand<can_imu_id_t, IMU_ENUM_MAX - 1>(); // Limit to valid IMU IDs
		std::uint16_t linear_accel_before = rockettest_rand<std::uint16_t>();          // Full 16-bit range
		std::uint16_t angular_velocity_before = rockettest_rand<std::uint16_t>();      // Full 16-bit range

		// Build message from test data
		build_imu_data_msg(prio_before, timestamp_before, axis_before, imu_id_before,
						   linear_accel_before, angular_velocity_before, &msg);

		// Extract data from built message
		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_imu_id_t imu_id_after;
		char axis_after;
		std::uint16_t linear_accel_after;
		std::uint16_t angular_velocity_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_imu_mag_id_dimension(&msg, &imu_id_after, &axis_after);
		get_imu_data(&msg, &linear_accel_after, &angular_velocity_after);

		// Verify round-trip consistency: all data should match original values
		rockettest_assert(msg_is_sensor_data_after == true);                   // Message correctly identified as sensor data
		rockettest_assert(type_after == MSG_SENSOR_IMU_X);                     // Message type preserved (X axis)
		rockettest_assert(timestamp_after == timestamp_before);                // Timestamp preserved
		rockettest_assert(imu_id_after == imu_id_before);                      // IMU ID preserved
		rockettest_assert(axis_after == axis_before);                          // Axis dimension preserved
		rockettest_assert(linear_accel_after == linear_accel_before);          // Acceleration value preserved
		rockettest_assert(angular_velocity_after == angular_velocity_before);  // Angular velocity preserved

		return test_passed;
	}
};

imu_data_msg_test imu_data_msg_test_inst;

/*
 * Magnetometer Data Message Test (MSG_SENSOR_MAG_X/Y/Z)
 *
 * Tests magnetometer (compass/magnetic field sensor) data for orientation and navigation.
 * Magnetometers send separate messages for X, Y, Z axes. This test uses Y axis as
 * representative example (X and Z follow identical format with different axis).
 *
 * Message format:
 * - Priority: 2 bits (0-3)
 * - Timestamp: 16 bits (2 bytes) - milliseconds
 * - Axis: char (X, Y, or Z) - encoded in message type (MSG_SENSOR_MAG_X/Y/Z)
 * - IMU ID: 8 bits (1 byte) - magnetometer is integrated with IMU
 * - Magnetic field value: 16 bits (2 bytes) - in Gauss or Tesla (scaled)
 */
class mag_data_msg_test : rockettest_test {
public:
	mag_data_msg_test() : rockettest_test("mag_data_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		// Generate random test data
		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();           // 0x3 = 2-bit mask (priority range 0-3)
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();             // Full 16-bit range
		char axis_before = 'Y';                                                        // Testing Y axis (X/Z use same pattern)
		can_imu_id_t imu_id_before = rockettest_rand<can_imu_id_t, IMU_ENUM_MAX - 1>(); // Limit to valid IMU IDs
		std::uint16_t mag_value_before = rockettest_rand<std::uint16_t>();             // Full 16-bit range

		// Build message from test data
		build_mag_data_msg(prio_before, timestamp_before, axis_before, imu_id_before,
						   mag_value_before, &msg);

		// Extract data from built message
		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_imu_id_t imu_id_after;
		char axis_after;
		std::uint16_t mag_value_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_imu_mag_id_dimension(&msg, &imu_id_after, &axis_after);
		get_mag_data(&msg, &mag_value_after);

		// Verify round-trip consistency: all data should match original values
		rockettest_assert(msg_is_sensor_data_after == true);           // Message correctly identified as sensor data
		rockettest_assert(type_after == MSG_SENSOR_MAG_Y);             // Message type preserved (Y axis)
		rockettest_assert(timestamp_after == timestamp_before);        // Timestamp preserved
		rockettest_assert(imu_id_after == imu_id_before);              // IMU ID preserved
		rockettest_assert(axis_after == axis_before);                  // Axis dimension preserved
		rockettest_assert(mag_value_after == mag_value_before);        // Magnetic field value preserved

		return test_passed;
	}
};

mag_data_msg_test mag_data_msg_test_inst;

/*
 * Barometer Data Message Test (MSG_SENSOR_BARO)
 *
 * Tests barometric pressure and temperature data from barometric altimeter sensors.
 * IMPORTANT: Pressure field is limited to 24 bits (not full 32-bit) due to CAN message
 * size constraints. This provides range of 0 to 16,777,215 units.
 *
 * Message format:
 * - Priority: 2 bits (0-3)
 * - Timestamp: 16 bits (2 bytes) - milliseconds
 * - IMU ID: 8 bits (1 byte) - barometer is integrated with IMU
 * - Pressure: 32 bits (4 bytes, only 24 bits used) - in Pascals or mbar (scaled)
 * - Temperature: 16 bits (2 bytes) - barometer's internal temperature sensor
 */
class baro_data_msg_test : rockettest_test {
public:
	baro_data_msg_test() : rockettest_test("baro_data_msg_test") {}

	bool run_test() override {
		bool test_passed = true;

		can_msg_t msg;

		// Generate random test data
		can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();           // 0x3 = 2-bit mask (priority range 0-3)
		std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();             // Full 16-bit range
		can_imu_id_t imu_id_before = rockettest_rand<can_imu_id_t, IMU_ENUM_MAX - 1>(); // Limit to valid IMU IDs
		std::uint32_t pressure_before = rockettest_rand<std::uint32_t, 0xFFFFFF>();    // 0xFFFFFF = 24-bit mask (hardware limitation)
		std::uint16_t temp_before = rockettest_rand<std::uint16_t>();                  // Full 16-bit range

		// Build message from test data
		build_baro_data_msg(prio_before, timestamp_before, imu_id_before, pressure_before,
							temp_before, &msg);

		// Extract data from built message
		bool msg_is_sensor_data_after;
		can_msg_type_t type_after;
		std::uint16_t timestamp_after;
		can_imu_id_t imu_id_after;
		std::uint32_t pressure_after;
		std::uint16_t temp_after;

		msg_is_sensor_data_after = msg_is_sensor_data(&msg);
		type_after = get_message_type(&msg);
		timestamp_after = get_timestamp(&msg);
		get_baro_data(&msg, &imu_id_after, &pressure_after, &temp_after);

		// Verify round-trip consistency: all data should match original values
		rockettest_assert(msg_is_sensor_data_after == true);           // Message correctly identified as sensor data
		rockettest_assert(type_after == MSG_SENSOR_BARO);              // Message type preserved
		rockettest_assert(timestamp_after == timestamp_before);        // Timestamp preserved
		rockettest_assert(imu_id_after == imu_id_before);              // IMU ID preserved
		rockettest_assert(pressure_after == pressure_before);          // Pressure value preserved (24-bit)
		rockettest_assert(temp_after == temp_before);                  // Temperature value preserved

		return test_passed;
	}
};

baro_data_msg_test baro_data_msg_test_inst;
