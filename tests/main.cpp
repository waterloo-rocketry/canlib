#include <cstdint>
#include <cstdio>

#include "can.h"
#include "message/msg_general.h"
#include "message_types.h"

void print_logger_log_line(const can_msg_t *msg) {
    std::printf("%08X", msg->sid);
    for (uint8_t i = 0; i < msg->data_len; i++) {
        std::printf("%02X", msg->data[i]);
    }
    std::putchar('\n');
}

int main(void) {
    can_msg_t msg;

    for (int i = 0; i < 256; i++) {
        build_general_board_status_msg(PRIO_HIGHEST, 12345, i, 6, &msg);
        print_logger_log_line(&msg);
    }

    return 0;
}
