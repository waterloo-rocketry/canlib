COMMON_C_SRCS := \
	message/msg_actuator.c \
	message/msg_general.c \
	message/msg_gps.c \
	message/msg_recovery.c \
	message/msg_sensor.c \
	message/msg_stream.c \
	util/can_rcv_buffer.c \
	util/can_tx_buffer.c \
	util/safe_ring_buffer.c \
	util/timing_util.c

COMMON_C_HEADERS := \
	message/msg_actuator.h \
	message/msg_common.h \
	message/msg_general.h \
	message/msg_gps.h \
	message/msg_recovery.h \
	message/msg_sensor.h \
	message/msg_stream.h \
	util/can_rcv_buffer.h \
	util/can_tx_buffer.h \
	util/safe_ring_buffer.h \
	util/timing_util.h

PIC18_C_SRCS := pic18f26k83/pic18f26k83_can.c

PIC18_C_HEADERS := pic18f26k83/pic18f26k83_can.h

STM32H7_C_SRCS := stm32h7/stm32h7_can.c

STM32H7_C_HEADERS := stm32h7/stm32h7_can.h

INCLUDE_PATHS := . rocketlib/include

TEST_SRCS := \
	tests/test_msg_actuator.cpp \
	tests/test_msg_common.cpp \
	tests/test_msg_general.cpp \
	tests/test_msg_gps.cpp \
	tests/test_msg_recovery.cpp \
	tests/test_msg_sensor.cpp \
	tests/test_msg_stream.cpp \
	tests/test_nullptr_reject.cpp \
	tests/test_tx_rcv_buffer.cpp

ROCKETLIB_SUBMODULE_PATH := rocketlib

include rocketlib/flows/firmware-library.mk
