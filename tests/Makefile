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

SRCS := \
	main.c \
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

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

unit_test: $(OBJS)
	$(CC) $^ -o $@

-include $(DEPS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(DEPS) *.o unit_test
