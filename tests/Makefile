CANLIB_PATH := ..

CFLAGS := \
	-std=c99 \
	-Wall \
	-Wextra \
	-pedantic \
	-MMD \
	-O2 \
	-I. \
	-I$(CANLIB_PATH) \
	-DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_ARMING \
	-DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_ROCKET

ifeq ($(COVERAGE), 1)

CFLAGS += \
	-fprofile-arcs \
	-ftest-coverage

LDFLAGS += -Wl,-lgcov

endif

CXXFLAGS := \
	-std=c++20 \
	-Wall \
	-Wextra \
	-pedantic \
	-MMD \
	-O2 \
	-I. \
	-I$(CANLIB_PATH) \
	-DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_ARMING \
	-DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_ROCKET

CANLIB_SRCS := \
	$(CANLIB_PATH)/message/msg_actuator.c \
	$(CANLIB_PATH)/message/msg_general.c \
	$(CANLIB_PATH)/message/msg_gps.c \
	$(CANLIB_PATH)/message/msg_recovery.c \
	$(CANLIB_PATH)/message/msg_sensor.c \
	$(CANLIB_PATH)/message/msg_stream.c \
	$(CANLIB_PATH)/util/can_rcv_buffer.c \
	$(CANLIB_PATH)/util/can_tx_buffer.c \
	$(CANLIB_PATH)/util/safe_ring_buffer.c \
	$(CANLIB_PATH)/util/timing_util.c

TEST_SRCS := \
	rockettest.cpp \
	test_msg_sensor.cpp \
	test_msg_common.cpp

CANLIB_OBJS = $(CANLIB_SRCS:.c=.o)
CANLIB_DEPS = $(CANLIB_SRCS:.c=.d)

TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST_DEPS = $(TEST_SRCS:.cpp=.d)

unit_test: $(CANLIB_OBJS) $(TEST_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

.PHONY: run_test
run_test: unit_test
	./unit_test

ifeq ($(COVERAGE), 1)

coverage.info: run_test
	lcov --capture --branch-coverage --directory ../message --directory ../util --output-file $@

html: coverage.info
	genhtml $^ --branch-coverage --output-directory $@

endif

-include $(CANLIB_DEPS)
-include $(TEST_DEPS)

.PHONY: clean
clean:
	rm -rf $(CANLIB_OBJS) $(CANLIB_DEPS) $(TEST_OBJS) $(TEST_DEPS) *.o ../message/*.gcda ../message/*.gcno ../util/*.gcda ../util/*.gcno unit_test coverage.info html
