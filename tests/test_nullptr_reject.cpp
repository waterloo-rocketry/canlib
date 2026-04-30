#include <cstdint>

#include "can.h"
#include "rockettest.hpp"

#include "message/msg_actuator.h"
#include "message/msg_common.h"
#include "message/msg_general.h"
#include "message/msg_gps.h"
#include "message/msg_recovery.h"
#include "message/msg_sensor.h"
#include "message/msg_stream.h"
#include "util/can_rcv_buffer.h"
#include "util/can_tx_buffer.h"
#include "util/safe_ring_buffer.h"
#include "util/timing_util.h"

namespace {

	template <typename T = void> T *notnullptr() {
		return reinterpret_cast<T *>((void *)1);
	}

} // namespace

class msg_ptr_nullptr_reject_test : rockettest_test {
public:
	msg_ptr_nullptr_reject_test() : rockettest_test("msg_ptr_nullptr_reject_test") {}

	bool run_test() override {
		bool test_passed = true;

		// -----------------------
		// 	msg_common tests
		// -----------------------
		rockettest_check_assert_triggered([] { get_message_type(nullptr); });
		rockettest_check_assert_triggered([] { get_board_type_unique_id(nullptr); });
		rockettest_check_assert_triggered([] { get_board_inst_unique_id(nullptr); });
		rockettest_check_assert_triggered([] { get_message_metadata(nullptr); });
		rockettest_check_assert_triggered([] { write_timestamp(0, nullptr); });
		rockettest_check_assert_triggered([] { get_timestamp(nullptr); });

		// -----------------------
		// msg_actuator tests
		// -----------------------
		rockettest_check_assert_triggered([] {
			build_actuator_cmd_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(),
								   0,
								   rockettest_rand_field<can_actuator_id_t, 0xff>(),
								   rockettest_rand_field<can_actuator_state_t, 0xff>(),
								   nullptr);
		});
		rockettest_check_assert_triggered([] {
			build_actuator_status_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(),
									  0,
									  rockettest_rand_field<can_actuator_id_t, 0xff>(),
									  rockettest_rand_field<can_actuator_state_t, 0xff>(),
									  rockettest_rand_field<can_actuator_state_t, 0xff>(),
									  nullptr);
		});

		rockettest_check_assert_triggered(
			[] { get_actuator_id(nullptr, notnullptr<can_actuator_id_t>()); });
		rockettest_check_assert_triggered(
			[] { get_actuator_id(notnullptr<can_msg_t>(), nullptr); });
		rockettest_check_assert_triggered([] { get_actuator_id(nullptr, nullptr); });

		rockettest_check_assert_triggered(
			[] { get_curr_actuator_state(nullptr, notnullptr<can_actuator_state_t>()); });
		rockettest_check_assert_triggered(
			[] { get_curr_actuator_state(notnullptr<can_msg_t>(), nullptr); });
		rockettest_check_assert_triggered([] { get_curr_actuator_state(nullptr, nullptr); });

		rockettest_check_assert_triggered(
			[] { get_cmd_actuator_state(nullptr, notnullptr<can_actuator_state_t>()); });
		rockettest_check_assert_triggered(
			[] { get_cmd_actuator_state(notnullptr<can_msg_t>(), nullptr); });
		rockettest_check_assert_triggered([] { get_cmd_actuator_state(nullptr, nullptr); });

		// -----------------------
		// msg_general tests
		// -----------------------
		rockettest_check_assert_triggered([] {
			build_general_board_status_msg(
				rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, nullptr);
		});

		rockettest_check_assert_triggered([] {
			build_reset_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, 0, nullptr);
		});

		rockettest_check_assert_triggered([] {
			build_debug_raw_msg(
				rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, nullptr, notnullptr<can_msg_t>());
		});
		rockettest_check_assert_triggered([] {
			build_debug_raw_msg(
				rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, notnullptr<uint8_t>(), nullptr);
		});
		rockettest_check_assert_triggered([] {
			build_debug_raw_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, nullptr, nullptr);
		});

		rockettest_check_assert_triggered([] {
			build_config_set_msg(
				rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, 0, 0, 0, nullptr);
		});

		rockettest_check_assert_triggered([] {
			build_config_status_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, 0, nullptr);
		});

		rockettest_check_assert_triggered(
			[] { get_general_board_status(nullptr, notnullptr<uint32_t>()); });
		rockettest_check_assert_triggered(
			[] { get_general_board_status(notnullptr<can_msg_t>(), nullptr); });
		rockettest_check_assert_triggered([] { get_general_board_status(nullptr, nullptr); });

		rockettest_check_assert_triggered(
			[] { get_reset_board_id(nullptr, notnullptr<uint8_t>(), notnullptr<uint8_t>()); });
		rockettest_check_assert_triggered(
			[] { get_reset_board_id(notnullptr<can_msg_t>(), nullptr, notnullptr<uint8_t>()); });
		rockettest_check_assert_triggered(
			[] { get_reset_board_id(notnullptr<can_msg_t>(), notnullptr<uint8_t>(), nullptr); });
		rockettest_check_assert_triggered([] { get_reset_board_id(nullptr, nullptr, nullptr); });

		rockettest_check_assert_triggered(
			[] { check_board_need_reset(nullptr, notnullptr<bool>()); });
		rockettest_check_assert_triggered(
			[] { check_board_need_reset(notnullptr<can_msg_t>(), nullptr); });
		rockettest_check_assert_triggered([] { check_board_need_reset(nullptr, nullptr); });

		rockettest_check_assert_triggered(
			[] { get_debug_raw_data(nullptr, notnullptr<uint8_t>()); });
		rockettest_check_assert_triggered(
			[] { get_debug_raw_data(notnullptr<can_msg_t>(), nullptr); });
		rockettest_check_assert_triggered([] { get_debug_raw_data(nullptr, nullptr); });

		rockettest_check_assert_triggered([] {
			get_config_set_target_board(nullptr, notnullptr<uint8_t>(), notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_config_set_target_board(notnullptr<can_msg_t>(), nullptr, notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_config_set_target_board(notnullptr<can_msg_t>(), notnullptr<uint8_t>(), nullptr);
		});
		rockettest_check_assert_triggered(
			[] { get_config_set_target_board(nullptr, nullptr, nullptr); });

		rockettest_check_assert_triggered(
			[] { get_config_id_value(nullptr, notnullptr<uint16_t>(), notnullptr<uint16_t>()); });
		rockettest_check_assert_triggered(
			[] { get_config_id_value(notnullptr<can_msg_t>(), nullptr, notnullptr<uint16_t>()); });
		rockettest_check_assert_triggered(
			[] { get_config_id_value(notnullptr<can_msg_t>(), notnullptr<uint16_t>(), nullptr); });
		rockettest_check_assert_triggered([] { get_config_id_value(nullptr, nullptr, nullptr); });

		// -----------------------
		// msg_gps tests
		// -----------------------
		rockettest_check_assert_triggered([] {
			build_gps_time_msg(
				rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, 0, 0, 0, nullptr);
		});
		rockettest_check_assert_triggered([] {
			build_gps_lat_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, 0, 0, 0, nullptr);
		});
		rockettest_check_assert_triggered([] {
			build_gps_lon_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, 0, 0, 0, nullptr);
		});
		rockettest_check_assert_triggered([] {
			build_gps_alt_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, 0, nullptr);
		});
		rockettest_check_assert_triggered([] {
			build_gps_info_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, 0, nullptr);
		});

		rockettest_check_assert_triggered([] {
			get_gps_time(nullptr,
						 notnullptr<uint8_t>(),
						 notnullptr<uint8_t>(),
						 notnullptr<uint8_t>(),
						 notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_time(notnullptr<can_msg_t>(),
						 nullptr,
						 notnullptr<uint8_t>(),
						 notnullptr<uint8_t>(),
						 notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_time(notnullptr<can_msg_t>(),
						 notnullptr<uint8_t>(),
						 nullptr,
						 notnullptr<uint8_t>(),
						 notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_time(notnullptr<can_msg_t>(),
						 notnullptr<uint8_t>(),
						 notnullptr<uint8_t>(),
						 nullptr,
						 notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_time(notnullptr<can_msg_t>(),
						 notnullptr<uint8_t>(),
						 notnullptr<uint8_t>(),
						 notnullptr<uint8_t>(),
						 nullptr);
		});
		rockettest_check_assert_triggered(
			[] { get_gps_time(nullptr, nullptr, nullptr, nullptr, nullptr); });

		rockettest_check_assert_triggered([] {
			get_gps_lat(nullptr,
						notnullptr<uint8_t>(),
						notnullptr<uint8_t>(),
						notnullptr<uint16_t>(),
						notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_lat(notnullptr<can_msg_t>(),
						nullptr,
						notnullptr<uint8_t>(),
						notnullptr<uint16_t>(),
						notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_lat(notnullptr<can_msg_t>(),
						notnullptr<uint8_t>(),
						nullptr,
						notnullptr<uint16_t>(),
						notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_lat(notnullptr<can_msg_t>(),
						notnullptr<uint8_t>(),
						notnullptr<uint8_t>(),
						nullptr,
						notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_lat(notnullptr<can_msg_t>(),
						notnullptr<uint8_t>(),
						notnullptr<uint8_t>(),
						notnullptr<uint16_t>(),
						nullptr);
		});
		rockettest_check_assert_triggered(
			[] { get_gps_lat(nullptr, nullptr, nullptr, nullptr, nullptr); });

		// get_gps_lon
		rockettest_check_assert_triggered([] {
			get_gps_lon(nullptr,
						notnullptr<uint8_t>(),
						notnullptr<uint8_t>(),
						notnullptr<uint16_t>(),
						notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_lon(notnullptr<can_msg_t>(),
						nullptr,
						notnullptr<uint8_t>(),
						notnullptr<uint16_t>(),
						notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_lon(notnullptr<can_msg_t>(),
						notnullptr<uint8_t>(),
						nullptr,
						notnullptr<uint16_t>(),
						notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_lon(notnullptr<can_msg_t>(),
						notnullptr<uint8_t>(),
						notnullptr<uint8_t>(),
						nullptr,
						notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_gps_lon(notnullptr<can_msg_t>(),
						notnullptr<uint8_t>(),
						notnullptr<uint8_t>(),
						notnullptr<uint16_t>(),
						nullptr);
		});
		rockettest_check_assert_triggered(
			[] { get_gps_lon(nullptr, nullptr, nullptr, nullptr, nullptr); });

		// get_gps_alt
		rockettest_check_assert_triggered(
			[] { get_gps_alt(nullptr, notnullptr<uint32_t>(), notnullptr<uint8_t>()); });
		rockettest_check_assert_triggered(
			[] { get_gps_alt(notnullptr<can_msg_t>(), nullptr, notnullptr<uint8_t>()); });
		rockettest_check_assert_triggered(
			[] { get_gps_alt(notnullptr<can_msg_t>(), notnullptr<uint32_t>(), nullptr); });
		rockettest_check_assert_triggered([] { get_gps_alt(nullptr, nullptr, nullptr); });

		// get_gps_info
		rockettest_check_assert_triggered(
			[] { get_gps_info(nullptr, notnullptr<uint8_t>(), notnullptr<uint8_t>()); });
		rockettest_check_assert_triggered(
			[] { get_gps_info(notnullptr<can_msg_t>(), nullptr, notnullptr<uint8_t>()); });
		rockettest_check_assert_triggered(
			[] { get_gps_info(notnullptr<can_msg_t>(), notnullptr<uint8_t>(), nullptr); });
		rockettest_check_assert_triggered([] { get_gps_info(nullptr, nullptr, nullptr); });

		// -----------------------
		// msg_recovery tests
		// -----------------------
		rockettest_check_assert_triggered([] {
			build_alt_arm_cmd_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(),
								  0,
								  rockettest_rand_field<can_altimeter_id_t, 0xff>(),
								  rockettest_rand_field<can_alt_arm_state_t, 0xff>(),
								  nullptr);
		});

		rockettest_check_assert_triggered([] {
			build_alt_arm_status_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(),
									 0,
									 rockettest_rand_field<can_altimeter_id_t, 0xff>(),
									 rockettest_rand_field<can_alt_arm_state_t, 0xff>(),
									 0,
									 0,
									 nullptr);
		});

		rockettest_check_assert_triggered([] {
			get_alt_arm_state(
				nullptr, notnullptr<can_altimeter_id_t>(), notnullptr<can_alt_arm_state_t>());
		});
		rockettest_check_assert_triggered([] {
			get_alt_arm_state(notnullptr<can_msg_t>(), nullptr, notnullptr<can_alt_arm_state_t>());
		});
		rockettest_check_assert_triggered([] {
			get_alt_arm_state(notnullptr<can_msg_t>(), notnullptr<can_altimeter_id_t>(), nullptr);
		});
		rockettest_check_assert_triggered([] { get_alt_arm_state(nullptr, nullptr, nullptr); });

		rockettest_check_assert_triggered(
			[] { get_pyro_voltage_data(nullptr, notnullptr<uint16_t>(), notnullptr<uint16_t>()); });
		rockettest_check_assert_triggered([] {
			get_pyro_voltage_data(notnullptr<can_msg_t>(), nullptr, notnullptr<uint16_t>());
		});
		rockettest_check_assert_triggered([] {
			get_pyro_voltage_data(notnullptr<can_msg_t>(), notnullptr<uint16_t>(), nullptr);
		});
		rockettest_check_assert_triggered([] { get_pyro_voltage_data(nullptr, nullptr, nullptr); });

		// -----------------------
		// msg_sensor tests
		// -----------------------
		rockettest_check_assert_triggered([] {
			build_analog_sensor_16bit_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(),
										  0,
										  rockettest_rand_field<can_analog_sensor_id_t, 0xff>(),
										  0,
										  nullptr);
		});
		rockettest_check_assert_triggered([] {
			build_analog_sensor_32bit_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(),
										  0,
										  rockettest_rand_field<can_analog_sensor_id_t, 0xff>(),
										  0,
										  nullptr);
		});
		rockettest_check_assert_triggered([] {
			build_3d_analog_sensor_16bit_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(),
											 0,
											 rockettest_rand_field<can_dem_3d_sensor_id_t, 0xff>(),
											 0,
											 0,
											 0,
											 nullptr);
		});
		rockettest_check_assert_triggered([] {
			build_2d_analog_sensor_24bit_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(),
											 0,
											 rockettest_rand_field<can_dem_2d_sensor_id_t, 0xff>(),
											 0,
											 0,
											 nullptr);
		});

		rockettest_check_assert_triggered([] { msg_is_analog_sensor(nullptr); });

		rockettest_check_assert_triggered([] {
			get_analog_sensor_data_16bit(
				nullptr, notnullptr<can_analog_sensor_id_t>(), notnullptr<uint16_t>());
		});
		rockettest_check_assert_triggered([] {
			get_analog_sensor_data_16bit(notnullptr<can_msg_t>(), nullptr, notnullptr<uint16_t>());
		});
		rockettest_check_assert_triggered([] {
			get_analog_sensor_data_16bit(
				notnullptr<can_msg_t>(), notnullptr<can_analog_sensor_id_t>(), nullptr);
		});
		rockettest_check_assert_triggered(
			[] { get_analog_sensor_data_16bit(nullptr, nullptr, nullptr); });

		rockettest_check_assert_triggered([] {
			get_analog_sensor_data_32bit(
				nullptr, notnullptr<can_analog_sensor_id_t>(), notnullptr<uint32_t>());
		});
		rockettest_check_assert_triggered([] {
			get_analog_sensor_data_32bit(notnullptr<can_msg_t>(), nullptr, notnullptr<uint32_t>());
		});
		rockettest_check_assert_triggered([] {
			get_analog_sensor_data_32bit(
				notnullptr<can_msg_t>(), notnullptr<can_analog_sensor_id_t>(), nullptr);
		});
		rockettest_check_assert_triggered(
			[] { get_analog_sensor_data_32bit(nullptr, nullptr, nullptr); });

		rockettest_check_assert_triggered([] {
			get_3d_analog_sensor_data_16bit(nullptr,
											notnullptr<can_dem_3d_sensor_id_t>(),
											notnullptr<uint16_t>(),
											notnullptr<uint16_t>(),
											notnullptr<uint16_t>());
		});
		rockettest_check_assert_triggered([] {
			get_3d_analog_sensor_data_16bit(notnullptr<can_msg_t>(),
											nullptr,
											notnullptr<uint16_t>(),
											notnullptr<uint16_t>(),
											notnullptr<uint16_t>());
		});
		rockettest_check_assert_triggered([] {
			get_3d_analog_sensor_data_16bit(notnullptr<can_msg_t>(),
											notnullptr<can_dem_3d_sensor_id_t>(),
											nullptr,
											notnullptr<uint16_t>(),
											notnullptr<uint16_t>());
		});
		rockettest_check_assert_triggered([] {
			get_3d_analog_sensor_data_16bit(notnullptr<can_msg_t>(),
											notnullptr<can_dem_3d_sensor_id_t>(),
											notnullptr<uint16_t>(),
											nullptr,
											notnullptr<uint16_t>());
		});
		rockettest_check_assert_triggered([] {
			get_3d_analog_sensor_data_16bit(notnullptr<can_msg_t>(),
											notnullptr<can_dem_3d_sensor_id_t>(),
											notnullptr<uint16_t>(),
											notnullptr<uint16_t>(),
											nullptr);
		});
		rockettest_check_assert_triggered(
			[] { get_3d_analog_sensor_data_16bit(nullptr, nullptr, nullptr, nullptr, nullptr); });

		rockettest_check_assert_triggered([] {
			get_2d_analog_sensor_data_24bit(nullptr,
											notnullptr<can_dem_2d_sensor_id_t>(),
											notnullptr<uint32_t>(),
											notnullptr<uint32_t>());
		});
		rockettest_check_assert_triggered([] {
			get_2d_analog_sensor_data_24bit(
				notnullptr<can_msg_t>(), nullptr, notnullptr<uint32_t>(), notnullptr<uint32_t>());
		});
		rockettest_check_assert_triggered([] {
			get_2d_analog_sensor_data_24bit(notnullptr<can_msg_t>(),
											notnullptr<can_dem_2d_sensor_id_t>(),
											nullptr,
											notnullptr<uint32_t>());
		});
		rockettest_check_assert_triggered([] {
			get_2d_analog_sensor_data_24bit(notnullptr<can_msg_t>(),
											notnullptr<can_dem_2d_sensor_id_t>(),
											notnullptr<uint32_t>(),
											nullptr);
		});
		rockettest_check_assert_triggered(
			[] { get_2d_analog_sensor_data_24bit(nullptr, nullptr, nullptr, nullptr); });

		// -----------------------
		// msg_stream tests
		// -----------------------
		rockettest_check_assert_triggered([] {
			build_stream_status_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, 0, nullptr);
		});

		rockettest_check_assert_triggered([] {
			build_stream_data_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(),
								  0,
								  0,
								  nullptr,
								  0,
								  notnullptr<can_msg_t>());
		});
		rockettest_check_assert_triggered([] {
			build_stream_data_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(),
								  0,
								  0,
								  notnullptr<uint8_t>(),
								  0,
								  nullptr);
		});
		rockettest_check_assert_triggered([] {
			build_stream_data_msg(
				rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, nullptr, 0, nullptr);
		});

		rockettest_check_assert_triggered([] {
			build_stream_retry_msg(rockettest_rand_field<can_msg_prio_t, 0x3>(), 0, 0, nullptr);
		});

		rockettest_check_assert_triggered(
			[] { get_stream_status(nullptr, notnullptr<uint32_t>(), notnullptr<uint32_t>()); });
		rockettest_check_assert_triggered(
			[] { get_stream_status(notnullptr<can_msg_t>(), nullptr, notnullptr<uint32_t>()); });
		rockettest_check_assert_triggered(
			[] { get_stream_status(notnullptr<can_msg_t>(), notnullptr<uint32_t>(), nullptr); });
		rockettest_check_assert_triggered([] { get_stream_status(nullptr, nullptr, nullptr); });

		rockettest_check_assert_triggered([] {
			get_stream_data(
				nullptr, notnullptr<uint8_t>(), notnullptr<uint8_t>(), notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_stream_data(
				notnullptr<can_msg_t>(), nullptr, notnullptr<uint8_t>(), notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_stream_data(
				notnullptr<can_msg_t>(), notnullptr<uint8_t>(), nullptr, notnullptr<uint8_t>());
		});
		rockettest_check_assert_triggered([] {
			get_stream_data(
				notnullptr<can_msg_t>(), notnullptr<uint8_t>(), notnullptr<uint8_t>(), nullptr);
		});
		rockettest_check_assert_triggered(
			[] { get_stream_data(nullptr, nullptr, nullptr, nullptr); });

		rockettest_check_assert_triggered(
			[] { get_stream_retry_seq_id(nullptr, notnullptr<uint8_t>()); });
		rockettest_check_assert_triggered(
			[] { get_stream_retry_seq_id(notnullptr<can_msg_t>(), nullptr); });
		rockettest_check_assert_triggered([] { get_stream_retry_seq_id(nullptr, nullptr); });

		// -----------------------
		// safe_ring_buffer tests
		// -----------------------
		rockettest_check_assert_triggered([] { srb_init(nullptr, notnullptr(), 0, 0); });
		rockettest_check_assert_triggered([] { srb_init(notnullptr<srb_ctx_t>(), nullptr, 0, 0); });
		rockettest_check_assert_triggered([] { srb_init(nullptr, nullptr, 0, 0); });

		rockettest_check_assert_triggered([] { srb_push(nullptr, notnullptr()); });
		rockettest_check_assert_triggered([] { srb_push(notnullptr<srb_ctx_t>(), nullptr); });
		rockettest_check_assert_triggered([] { srb_push(nullptr, nullptr); });

		rockettest_check_assert_triggered([] { srb_is_full(nullptr); });
		rockettest_check_assert_triggered([] { srb_is_empty(nullptr); });

		rockettest_check_assert_triggered([] { srb_pop(nullptr, notnullptr()); });
		rockettest_check_assert_triggered([] { srb_pop(notnullptr<srb_ctx_t>(), nullptr); });
		rockettest_check_assert_triggered([] { srb_pop(nullptr, nullptr); });

		rockettest_check_assert_triggered([] { srb_peek(nullptr, notnullptr()); });
		rockettest_check_assert_triggered([] { srb_peek(notnullptr<srb_ctx_t>(), nullptr); });
		rockettest_check_assert_triggered([] { srb_peek(nullptr, nullptr); });

		// -----------------------
		// can_tx_buffer tests
		// -----------------------
		rockettest_check_assert_triggered([] {
			txb_init(nullptr, 0, notnullptr<void(const can_msg_t *)>(), notnullptr<bool()>());
		});
		rockettest_check_assert_triggered(
			[] { txb_init(notnullptr(), 0, nullptr, notnullptr<bool()>()); });
		rockettest_check_assert_triggered(
			[] { txb_init(notnullptr(), 0, notnullptr<void(const can_msg_t *)>(), nullptr); });
		rockettest_check_assert_triggered([] { txb_init(nullptr, 0, nullptr, nullptr); });

		rockettest_check_assert_triggered([] { txb_enqueue(nullptr); });

		// -----------------------
		// can_rx_buffer tests
		// -----------------------
		rockettest_check_assert_triggered([] { rcvb_init(nullptr, 0); });
		rockettest_check_assert_triggered([] { rcvb_push_message(nullptr); });
		rockettest_check_assert_triggered([] { rcvb_pop_message(nullptr); });
		rockettest_check_assert_triggered([] { rcvb_peek_message(nullptr); });

		// -----------------------
		// timing_util tests
		// -----------------------
		rockettest_check_assert_triggered([] { can_generate_timing_params(48000000, nullptr); });
		return test_passed;
	}
};

msg_ptr_nullptr_reject_test msg_ptr_nullptr_reject_test_inst;
