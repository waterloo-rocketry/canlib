/*
 * stm32h733_can.h
 *
 *  Created on: Mar 16, 2024
 *      Author: joedo
 */
#include "stm32h7xx_hal.h"
#include "canlib.h"
#include <stdbool.h>

#ifndef STM32H733_STM32H733_CAN_H_
#define STM32H733_STM32H733_CAN_H_

/* CAN module should be automatically initialized by static void MX_FDCANx_Init
 * This library is deseigned to use CANRxFIFO0 by default, which is not filtered
 * TODO: For specific message IDs which need higher priority, the chip includes dedicated Rx buffers
 * Use can_buffer_init_stm() to bind a board and/or message ID to a callback function
 * TODO: add support for aribitrary FIFOs
 */

typedef void (*can_receive_callback)(const can_msg_t *message, uint32_t timestamp);

bool can_init_stm(FDCAN_HandleTypeDef *handle,  can_receive_callback receive_callback);

//Register an Rx buffer for a specific canlib board and/or message type

bool can_buffer_init_stm(FDCAN_HandleTypeDef *handle, uint16_t board_id, uint16_t msg_id, can_receive_callback receive_callback);

// send a CAN message
void can_send(const can_msg_t* message);

// returns true if the CAN module is ready to send a message
bool can_send_rdy(void);


#endif /* STM32H733_STM32H733_CAN_H_ */
