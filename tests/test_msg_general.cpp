#include <cstdint>

#include "rockettest.hpp"

#include "can.h"
#include "message/msg_common.h"
#include "message/msg_general.h"
#include "message_types.h"

class general_status_message_test : rockettest_test {
public:
    general_status_message_test() : rockettest_test("general_status_message_test") {}

    bool run_test() override {
        bool test_passed = true;

        can_msg_t msg;

        can_msg_prio_t prio_before = rockettest_rand<can_msg_prio_t, 0x3>();
        std::uint16_t timestamp_before = rockettest_rand<std::uint16_t>();
        std::uint32_t general_error_bitfield_before = rockettest_rand<std::uint32_t>();
        std::uint16_t board_error_bitfield_before = rockettest_rand<std::uint16_t>();

        build_general_board_status_msg(prio_before, timestamp_before, general_error_bitfield_before, board_error_bitfield_before, &msg);

        std::uint16_t timestamp_extracted;
        std::uint32_t general_error_bitfield_extracted;
        std::uint16_t board_error_bitfield_extracted;

        timestamp_extracted = (static_cast<std::uint16_t>(msg.data[0]) << 8) | msg.data[1];
        general_error_bitfield_extracted = (static_cast<std::uint32_t>(msg.data[2]) << 24) |
                                           (static_cast<std::uint32_t>(msg.data[3]) << 16) |
                                           (static_cast<std::uint32_t>(msg.data[4]) << 8) |
                                           (static_cast<std::uint32_t>(msg.data[5]) << 0);
        board_error_bitfield_extracted = (static_cast<std::uint16_t>(msg.data[6]) << 8) | msg.data[7];

        rockettest_check_expr_true(msg.data_len == 8);
        rockettest_check_expr_true(timestamp_extracted == timestamp_before);
        rockettest_check_expr_true(general_error_bitfield_extracted == general_error_bitfield_before);
        rockettest_check_expr_true(board_error_bitfield_extracted == board_error_bitfield_before);

        can_msg_type_t type_after;
        std::uint16_t timestamp_after;
        std::uint32_t general_error_bitfield_after;
        std::uint16_t board_error_bitfield_after;

        type_after = get_message_type(&msg);
        timestamp_after = get_timestamp(&msg);
        get_general_board_status(&msg, &general_error_bitfield_after, &board_error_bitfield_after);
        
        rockettest_check_expr_true(type_after == MSG_GENERAL_BOARD_STATUS);
        rockettest_check_expr_true(timestamp_after == timestamp_before);
        rockettest_check_expr_true(general_error_bitfield_after == general_error_bitfield_before);
        rockettest_check_expr_true(board_error_bitfield_after == board_error_bitfield_before);
        
        return test_passed;
    }
};

general_status_message_test general_status_message_test_inst;