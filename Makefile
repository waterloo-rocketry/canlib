COMMON_C_SRCS := \
	common/crc8.c \
	common/low_pass_filter.c \
	logging/mbr.c \
	can/msg_actuator.c \
	can/msg_canards.c \
	can/msg_general.c \
	can/msg_gps.c \
	can/msg_recovery.c \
	can/msg_sensor.c \
	can/msg_stream.c \
	can/msg_telemetry.c \
	buffer/can_rcv_buffer.c \
	buffer/can_tx_buffer.c \
	buffer/safe_ring_buffer.c \
	buffer/timing_util.c

COMMON_C_HEADERS := \
	include/common.h \
	include/crc8.h \
	include/electrical.h \
	include/low_pass_filter.h \
	include/mathops.h \
	include/mbr.h \
	include/message/msg_actuator.h \
	include/message/msg_canards.h \
	include/message/msg_common.h \
	include/message/msg_general.h \
	include/message/msg_gps.h \
	include/message/msg_recovery.h \
	include/message/msg_sensor.h \
	include/message/msg_stream.h \
	include/message/msg_telemetry.h \
	util/can_rcv_buffer.h \
	util/can_tx_buffer.h \
	util/safe_ring_buffer.h \
	util/timing_util.h

PIC18_C_SRCS := \
	pic18f26k83/timer.c \
	pic18f26k83/pic18f26k83_can.c

PIC18_C_HEADERS := \
	include/timer.h \
	pic18f26k83/pic18f26k83_can.h

STM32H7_C_SRCS := \
	stm32h7/littlefs_sd_shim.c \
	stm32h7/stm32h7_can.c

STM32H7_C_HEADERS := \
	include/stm32h7/littlefs_sd_shim.h \
	stm32h7/stm32h7_can.h

INCLUDE_PATHS := . include

TEST_SRCS := \
	tests/test_crc8.cpp \
	tests/test_low_pass_filter.cpp \
	tests/test_mathops.cpp \
	tests/test_mbr.cpp \
	tests/test_rockettest.cpp \
	tests/test_msg_actuator.cpp \
	tests/test_msg_canards.cpp \
	tests/test_msg_common.cpp \
	tests/test_msg_general.cpp \
	tests/test_msg_gps.cpp \
	tests/test_msg_recovery.cpp \
	tests/test_msg_sensor.cpp \
	tests/test_msg_stream.cpp \
	tests/test_msg_telemetry.cpp \
	tests/test_nullptr_reject.cpp \
	tests/test_timing_util.cpp \
	tests/test_tx_rcv_buffer.cpp

ROCKETLIB_SUBMODULE_PATH := .

EXTRA_C_CXX_FLAGS := -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_ARMING -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_ROCKET

include flows/firmware-library.mk
