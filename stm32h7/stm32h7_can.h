#ifndef CANLIB_STM32H7_CAN_H
#define CANLIB_STM32H7_CAN_H

#include <stdbool.h>

#include "stm32h7xx_hal.h"

#include "can.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start STM32H7 CAN Controller
 *
 * @param handle pointer to FDCAN_HandleTypeDef struct in `main.c`
 * @param receive_callback CAN message receive handler, the function would be called when a CAN
 * message is received
 * @return `true` if succeed
 * @note This depends on STM32CubeMX generated HAL code has already initialized CAN controller
 */
bool stm32h7_can_init(FDCAN_HandleTypeDef *handle,
					  void (*receive_callback)(const can_msg_t *message));

/**
 * @brief Send a CAN message
 *
 * @param message CAN message to be sent
 * @return `true` if succeed
 */
bool stm32h7_can_send(const can_msg_t *message);

/**
 * @brief Check if CAN controller is ready to send a message
 *
 * @return `true` if ready to send a message
 */
bool stm32h7_can_send_rdy(void);

#ifdef __cplusplus
}
#endif

#endif
