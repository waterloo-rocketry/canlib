CLANG_FORMAT := clang-format
CLANG_TIDY := clang-tidy

INCLUDE_PATH_C_CXX_FLAGS := $(foreach inc, $(INCLUDE_PATH), $(addprefix -I, $(inc)))
INCLUDE_PATH_CLANG_TIDY_FLAGS := $(foreach inc, $(INCLUDE_PATH), $(addprefix --extra-arg-before="-I, $(addsuffix ", $(inc))))

C_CXX_FLAGS := \
	$(INCLUDE_PATH_C_CXX_FLAGS) \
	-Wall \
	-Wextra \
	-pedantic \
	-MMD \
	-DUNIT_TEST

ifeq ($(DEBUG), 1)
C_CXX_FLAGS += -Og -g
else
C_CXX_FLAGS += -O2
endif

ifeq ($(COVERAGE), 1)
C_CXX_FLAGS += \
	-fprofile-arcs \
	-ftest-coverage \
	-fcondition-coverage \
	-fpath-coverage

LDFLAGS += -Wl,-lgcov
endif

CFLAGS := \
	$(C_CXX_FLAGS) \
	-std=c99

CXXFLAGS := \
	$(C_CXX_FLAGS) \
	-std=c++20 \
	-I$(ROCKETTEST_PATH)

CLANG_TIDY_FLAGS := \
	--warnings-as-errors="*" \
	--checks="clang-*,misc-*" \
	--extra-arg-before="-std=c99" \
	--extra-arg-before="-pedantic" \
	$(INCLUDE_PATH_CLANG_TIDY_FLAGS)

CPP_SRCS := \
	$(ROCKETTEST_PATH)/rockettest.cpp \
	$(TEST_SRCS)

ifeq ($(COVERAGE), 1)
TEST_BUILD_DIR := build/test-cov
else
TEST_BUILD_DIR := build/test
endif

COMMON_C_OBJS = $(patsubst %.c,$(TEST_BUILD_DIR)/%.o,$(COMMON_C_SRCS))
COMMON_C_DEPS = $(COMMON_C_SRCS:.c=.d)

CPP_OBJS = $(patsubst %.cpp,$(TEST_BUILD_DIR)/%.o,$(CPP_SRCS))
CPP_DEPS = $(CPP_SRCS:.cpp=.d)

$(TEST_BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_BUILD_DIR)/unit_test: $(COMMON_C_OBJS) $(CPP_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $^ $(LDFLAGS) -o $@

.PHONY: run_test
run_test: $(TEST_BUILD_DIR)/unit_test
	./$(TEST_BUILD_DIR)/unit_test

ifeq ($(COVERAGE), 1)

$(TEST_BUILD_DIR)/coverage.info: run_test
	lcov --capture --branch-coverage --mcdc-coverage --no-external --directory . -o $@

$(TEST_BUILD_DIR)/coverage-filtered.info: $(TEST_BUILD_DIR)/coverage.info
	lcov --remove $^ "*/tests/*" "*/rockettest/*" -o $@ --branch-coverage --mcdc-coverage

$(TEST_BUILD_DIR)/html: $(TEST_BUILD_DIR)/coverage-filtered.info
	genhtml $^ --flat --branch-coverage --mcdc-coverage --output-directory $@

endif

.PHONY: lint
lint:
	$(CLANG_TIDY) $(CLANG_TIDY_FLAGS) $(COMMON_C_SRCS) $(COMMON_C_HEADERS)

.PHONY: format
format:
	$(CLANG_FORMAT) -i $(COMMON_C_SRCS) $(COMMON_C_HEADERS) $(PIC18_C_SRCS) $(PIC18_C_HEADERS) $(STM32H7_C_SRCS) $(STM32H7_C_HEADERS) $(TEST_SRCS)

-include $(COMMON_C_DEPS)
-include $(CPP_DEPS)

.PHONY: clean
clean:
	rm -rf build
