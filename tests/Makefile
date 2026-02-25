CANLIB_PATH := ..

CFLAGS := \
	-std=c99 \
	-Wall \
	-Wextra \
	-pedantic \
	-MMD \
	-I. \
	-I$(CANLIB_PATH) \
	-DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_ARMING \
	-DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_ROCKET

CXXFLAGS := \
	-std=c++20 \
	-Wall \
	-Wextra \
	-pedantic \
	-MMD \
	-I. \
	-I$(CANLIB_PATH) \
	-DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_ARMING \
	-DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_ROCKET

ifeq ($(COVERAGE), 1)

CFLAGS += \
	-fprofile-arcs \
	-ftest-coverage

CXXFLAGS += \
	-fprofile-arcs \
	-ftest-coverage

LDFLAGS += -Wl,-lgcov

endif

ifeq ($(DEBUG), 1)

CFLAGS += -Og -g

CXXFLAGS += -Og -g

else

CFLAGS += -O2

CXXFLAGS += -O2

endif

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
	test_msg_common.cpp \
	test_nullptr_reject.cpp \
	test_msg_actuator.cpp \
	test_msg_gps.cpp \
	test_msg_general.cpp \
	test_msg_recovery.cpp \

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
	lcov --capture --branch-coverage --no-external --directory ../message --directory ../util --directory . -o $@

coverage-filtered.info: coverage.info
	lcov --remove $^ "*/tests/*" -o $@ --branch-coverage

html: coverage-filtered.info
	genhtml $^ --flat --branch-coverage --output-directory $@

endif

-include $(CANLIB_DEPS)
-include $(TEST_DEPS)

.PHONY: clean
clean:
	rm -rf $(CANLIB_OBJS) $(CANLIB_DEPS) $(TEST_OBJS) $(TEST_DEPS) *.o ../message/*.gcda ../message/*.gcno ../util/*.gcda ../util/*.gcno *.gcda *.gcno unit_test *.info html
