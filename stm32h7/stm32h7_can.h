#include <stdbool.h>

#include "stm32h7xx_hal.h"

#include "canlib.h"

#ifndef _STM32H7_CAN_H
#define _STM32H7_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

/* CAN module should be automatically initialized by static void MX_FDCANx_Init
 * This library is deseigned to use CANRxFIFO0 by default, which is not filtered
 * TODO: For specific message IDs which need higher priority, the chip includes dedicated Rx buffers
 * Use can_buffer_init_stm() to bind a board and/or message ID to a callback function
 * TODO: add support for aribitrary FIFOs
 */

typedef void (*can_receive_callback)(const can_msg_t *message, uint32_t timestamp);

bool can_init_stm(FDCAN_HandleTypeDef *handle, can_receive_callback receive_callback);

// TODO: Register an Rx buffer for a specific canlib board and/or message type
// bool can_buffer_init_stm(FDCAN_HandleTypeDef *handle, uint16_t board_id, uint16_t msg_id,
// can_receive_callback receive_callback);

// send a CAN message, return true if succeeed
bool can_send(const can_msg_t *message);

// returns true if the CAN module is ready to send a message
bool can_send_rdy(void);

#ifdef __cplusplus
}
#endif

#endif
