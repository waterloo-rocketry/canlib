CANLIB_PATH := ..

CFLAGS := \
	-Wall \
	-Wextra \
	-pedantic \
	-I$(CANLIB_PATH) \
	-DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_USB \
	-DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC

OBJS := \
	main.o \
	$(CANLIB_PATH)/message/msg_actuator.o \
	$(CANLIB_PATH)/message/msg_common.o \
	$(CANLIB_PATH)/message/msg_general.o \
	$(CANLIB_PATH)/message/msg_gps.o \
	$(CANLIB_PATH)/message/msg_recovery.o \
	$(CANLIB_PATH)/message/msg_sensor.o \
	$(CANLIB_PATH)/message/msg_state_est.o \
	$(CANLIB_PATH)/util/can_rcv_buffer.o \
	$(CANLIB_PATH)/util/can_tx_buffer.o \
	$(CANLIB_PATH)/util/safe_ring_buffer.o

unit_test: $(OBJS)
	$(CC) $^ -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) *.o unit_test
