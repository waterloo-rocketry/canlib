###########################
# Source File Path Variables
###########################

ROCKETTEST_SRCS := $(ROCKETLIB_SUBMODULE_PATH)/rockettest/rockettest.cpp
ROCKETTEST_HEADERS := $(ROCKETLIB_SUBMODULE_PATH)/rockettest/rockettest.hpp
ROCKETTEST_INCLUDE_PATH := $(ROCKETLIB_SUBMODULE_PATH)/rockettest

CPP_SRCS := \
	$(ROCKETTEST_SRCS) \
	$(TEST_SRCS)

###########################
# Common Build Variables
###########################

INCLUDE_PATHS_C_CXX_FLAGS := $(foreach inc, $(INCLUDE_PATHS), $(addprefix -I, $(inc)))

C_CXX_FLAGS += \
	$(INCLUDE_PATHS_C_CXX_FLAGS) \
	$(EXTRA_C_CXX_FLAGS) \
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

ifeq ($(filter run-test,$(MAKECMDGOALS)),run-test)
	BUILD_DIR := build/test
endif

ifneq ($(filter run-test-cov gen-cov-html,$(MAKECMDGOALS)),)
	BUILD_DIR := build/test-cov

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

#######################
# XC8 Compile Variables
#######################

XC8 := /opt/microchip/xc8/v3.10/bin/xc8-cc

ifeq ($(filter xc8-build,$(MAKECMDGOALS)),xc8-build)
	CC := $(XC8)
	CFLAGS := -mcpu=18F26K83 -mdfp=$(XC8_DFP_PATH) -mwarn=-9 -std=c99 -Wpedantic $(INCLUDE_PATHS_C_CXX_FLAGS) $(EXTRA_C_CXX_FLAGS)
	BUILD_DIR := build/xc8
endif

########################
# XC16 Compile Variables
########################

XC16 := /opt/microchip/xc16/v2.10/bin/xc16-gcc

ifeq ($(filter xc16-build,$(MAKECMDGOALS)),xc16-build)
	CC := $(XC16)
	CFLAGS := -mcpu=33EP256GP502 -std=c99 -Wall $(INCLUDE_PATHS_C_CXX_FLAGS) $(EXTRA_C_CXX_FLAGS)
	BUILD_DIR := build/xc16
endif

####################
# Linting Variables
####################

CLANG_TIDY := clang-tidy
INCLUDE_PATHS_CLANG_TIDY_FLAGS := $(foreach inc, $(INCLUDE_PATHS), $(addprefix --extra-arg-before="-I, $(addsuffix ", $(inc))))
EXTRA_C_CXX_CLANG_TIDY_FLAGS := $(foreach flag, $(EXTRA_C_CXX_FLAGS), $(addprefix --extra-arg-before=", $(addsuffix ", $(flag))))

CLANG_TIDY_FLAGS := \
	--warnings-as-errors="*" \
	--checks="clang-*,misc-*" \
	--extra-arg-before="-std=c99" \
	--extra-arg-before="-pedantic" \
	$(INCLUDE_PATHS_CLANG_TIDY_FLAGS) \
	$(EXTRA_C_CXX_CLANG_TIDY_FLAGS)

######################
# Formatting Variables
######################

CLANG_FORMAT := clang-format

###########################################
# Object File and Dependency File Variables
###########################################

COMMON_C_OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(COMMON_C_SRCS))
COMMON_C_DEPS = $(COMMON_C_SRCS:.c=.d)

PIC18_C_OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(PIC18_C_SRCS))
PIC18_C_DEPS = $(PIC18_C_SRCS:.c=.d)

CPP_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CPP_SRCS))
CPP_DEPS = $(CPP_SRCS:.cpp=.d)

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

.PHONY: run-test
run-test: $(BUILD_DIR)/unit_test
	./$(BUILD_DIR)/unit_test

###############################
# Unit Test with Coverage Build
###############################

.PHONY: run-test-cov
run-test-cov: $(BUILD_DIR)/unit_test
	./$(BUILD_DIR)/unit_test

$(BUILD_DIR)/coverage.info: run-test
	lcov --capture --branch-coverage --mcdc-coverage --no-external --directory . -o $@

$(BUILD_DIR)/coverage-filtered.info: $(BUILD_DIR)/coverage.info
	lcov --remove $^ "*/tests/*" "*/rockettest/*" -o $@ --branch-coverage --mcdc-coverage

$(BUILD_DIR)/html: $(BUILD_DIR)/coverage-filtered.info
	genhtml $^ --flat --branch-coverage --mcdc-coverage --output-directory $@

.PHONY: gen-cov-html
gen-cov-html: $(BUILD_DIR)/html
	true

####################
# XC8 Build
####################

.PHONY: xc8-build
xc8-build: $(COMMON_C_OBJS) $(PIC18_C_OBJS)
	true

####################
# XC16 Build
####################

.PHONY: xc16-build
xc16-build: $(COMMON_C_OBJS)
	true

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
	$(CLANG_FORMAT) --dry-run -Werror --style=file:$(ROCKETLIB_SUBMODULE_PATH)/.clang-format $(COMMON_C_SRCS) $(COMMON_C_HEADERS) $(PIC18_C_SRCS) $(PIC18_C_HEADERS) $(STM32H7_C_SRCS) $(STM32H7_C_HEADERS) $(TEST_SRCS) $(ROCKETTEST_SRCS) $(ROCKETTEST_HEADERS)

-include $(COMMON_C_DEPS)
-include $(CPP_DEPS)

####################
# Clean
####################

.PHONY: clean
clean:
	rm -rf build
