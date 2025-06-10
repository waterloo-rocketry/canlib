CANLIB_PATH := ..

CFLAGS := \
	-std=c99 \
	-Wall \
	-Wextra \
	-pedantic \
	-MMD \
	-O2 \
	-I$(CANLIB_PATH) \
	-DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_ARMING \
	-DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_ROCKET

CXXFLAGS := \
	-std=c++23 \
	-Wall \
	-Wextra \
	-pedantic \
	-MMD \
	-O2 \
	-I$(CANLIB_PATH) \
	-DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_ARMING \
	-DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_ROCKET

CANLIB_SRCS := \
	$(CANLIB_PATH)/message/msg_actuator.c \
	$(CANLIB_PATH)/message/msg_common.c \
	$(CANLIB_PATH)/message/msg_general.c \
	$(CANLIB_PATH)/message/msg_gps.c \
	$(CANLIB_PATH)/message/msg_recovery.c \
	$(CANLIB_PATH)/message/msg_sensor.c \
	$(CANLIB_PATH)/message/msg_state_est.c \
	$(CANLIB_PATH)/util/can_rcv_buffer.c \
	$(CANLIB_PATH)/util/can_tx_buffer.c \
	$(CANLIB_PATH)/util/safe_ring_buffer.c

TEST_SRCS := \
	main.c

CANLIB_OBJS = $(CANLIB_SRCS:.c=.o)
CANLIB_DEPS = $(CANLIB_SRCS:.c=.d)

TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_DEPS = $(TEST_SRCS:.c=.d)

unit_test: $(CANLIB_OBJS) $(TEST_OBJS)
	$(CXX) $^ -o $@

-include $(CANLIB_DEPS)
-include $(TEST_DEPS)

.PHONY: clean
clean:
	rm -f $(CANLIB_OBJS) $(CANLIB_DEPS) $(TEST_OBJS) $(TEST_DEPS) *.o unit_test
