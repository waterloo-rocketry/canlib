CLANG_FORMAT := clang-format
CLANG_TIDY := clang-tidy

INCLUDE_PATHS_C_CXX_FLAGS := $(foreach inc, $(INCLUDE_PATHS), $(addprefix -I, $(inc)))
INCLUDE_PATHS_CLANG_TIDY_FLAGS := $(foreach inc, $(INCLUDE_PATHS), $(addprefix --extra-arg-before="-I, $(addsuffix ", $(inc))))

C_CXX_FLAGS := \
	$(INCLUDE_PATHS_C_CXX_FLAGS) \
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
	-I$(ROCKETTEST_INCLUDE_PATH)

XC8_C_FLAGS := \
	-mcpu=18F26K83 \
	-mdfp=xc8 \
	-Iinclude \
	-c \
	-mwarn=-9 \
	-std=c99 \
	-Wpedantic

CLANG_TIDY_FLAGS := \
	--warnings-as-errors="*" \
	--checks="clang-*,misc-*" \
	--extra-arg-before="-std=c99" \
	--extra-arg-before="-pedantic" \
	$(INCLUDE_PATHS_CLANG_TIDY_FLAGS)

CPP_SRCS := \
	$(ROCKETTEST_SRCS) \
	$(TEST_SRCS)

ifeq ($(COVERAGE), 1)
BUILD_DIR := build/test-cov
else
BUILD_DIR := build/test
endif

COMMON_C_OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(COMMON_C_SRCS))
COMMON_C_DEPS = $(COMMON_C_SRCS:.c=.d)

CPP_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CPP_SRCS))
CPP_DEPS = $(CPP_SRCS:.cpp=.d)

#######################
# XC8 Compile Variables
#######################

XC8 := /opt/microchip/xc8/v3.10/bin/xc8-cc
xc8-build: CC := $(XC8)
xc8-build: CFLAGS := -mcpu=18F26K83 -mdfp=xc8 -Iinclude -mwarn=-9 -std=c99 -Wpedantic
xc8-build: BUILD_DIR := build/xc8

########################
# XC16 Compile Variables
########################

XC16 := /opt/microchip/xc16/v2.10/bin/xc16-gcc
xc16-build: CC := $(XC16)
xc16-build: CFLAGS := -mcpu=33EP256GP502 -Iinclude -std=c99 -Wall
xc16-build: BUILD_DIR := build/xc16

######################
# Object files compile
######################
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

####################
# Unit Test Build
####################
$(BUILD_DIR)/unit_test: $(COMMON_C_OBJS) $(CPP_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $^ $(LDFLAGS) -o $@

.PHONY: run_test
run_test: $(BUILD_DIR)/unit_test
	./$(BUILD_DIR)/unit_test

###############################
# Unit Test with Coverage Build
###############################

ifeq ($(COVERAGE), 1)

$(BUILD_DIR)/coverage.info: run_test
	lcov --capture --branch-coverage --mcdc-coverage --no-external --directory . -o $@

$(BUILD_DIR)/coverage-filtered.info: $(BUILD_DIR)/coverage.info
	lcov --remove $^ "*/tests/*" "*/rockettest/*" -o $@ --branch-coverage --mcdc-coverage

$(BUILD_DIR)/html: $(BUILD_DIR)/coverage-filtered.info
	genhtml $^ --flat --branch-coverage --mcdc-coverage --output-directory $@

endif

####################
# XC8 Build
####################

.PHONY: xc8-build
xc8-build: $(COMMON_C_OBJS)
	echo "$(CC) $(COMMON_C_OBJS)"

####################
# XC16 Build
####################

.PHONY: xc6-build
xc16-build: $(COMMON_C_OBJS)
	echo "$(CC) $(COMMON_C_OBJS)"

####################
# Linting
####################

.PHONY: lint
lint:
	$(CLANG_TIDY) $(CLANG_TIDY_FLAGS) $(COMMON_C_SRCS) $(COMMON_C_HEADERS)

####################
# Formatting
####################

.PHONY: format
format:
	$(CLANG_FORMAT) -i $(COMMON_C_SRCS) $(COMMON_C_HEADERS) $(PIC18_C_SRCS) $(PIC18_C_HEADERS) $(STM32H7_C_SRCS) $(STM32H7_C_HEADERS) $(TEST_SRCS) $(ROCKETTEST_SRCS) $(ROCKETTEST_HEADERS)

.PHONY: format-check
format-check:
	$(CLANG_FORMAT) --dry-run -Werror $(COMMON_C_SRCS) $(COMMON_C_HEADERS) $(PIC18_C_SRCS) $(PIC18_C_HEADERS) $(STM32H7_C_SRCS) $(STM32H7_C_HEADERS) $(TEST_SRCS) $(ROCKETTEST_SRCS) $(ROCKETTEST_HEADERS)

-include $(COMMON_C_DEPS)
-include $(CPP_DEPS)

####################
# Clean
####################

.PHONY: clean
clean:
	rm -rf build
