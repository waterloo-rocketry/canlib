COMMON_C_SRCS := \
	can_message/msg_actuator.c \
	can_message/msg_canards.c \
	can_message/msg_general.c \
	can_message/msg_gps.c \
	can_message/msg_recovery.c \
	can_message/msg_sensor.c \
	can_message/msg_stream.c \
	can_message/msg_telemetry.c \
	can_buffer/can_rcv_buffer.c \
	can_buffer/can_tx_buffer.c \
	can_buffer/safe_ring_buffer.c \
	can_buffer/timing_util.c

COMMON_C_HEADERS := \
	can_message/msg_actuator.h \
	can_message/msg_canards.h \
	can_message/msg_common.h \
	can_message/msg_general.h \
	can_message/msg_gps.h \
	can_message/msg_recovery.h \
	can_message/msg_sensor.h \
	can_message/msg_stream.h \
	can_message/msg_telemetry.h \
	can_buffer/can_rcv_buffer.h \
	can_buffer/can_tx_buffer.h \
	can_buffer/safe_ring_buffer.h \
	can_buffer/timing_util.h

PIC18_C_SRCS := pic18f26k83/pic18f26k83_can.c

PIC18_C_HEADERS := pic18f26k83/pic18f26k83_can.h

STM32H7_C_SRCS := stm32h7/stm32h7_can.c

STM32H7_C_HEADERS := stm32h7/stm32h7_can.h

INCLUDE_PATHS := . rocketlib/include

TEST_SRCS := \
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
