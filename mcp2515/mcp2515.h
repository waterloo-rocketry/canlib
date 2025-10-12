#ifndef MCP2515_MCP2515_H
#define MCP2515_MCP2515_H

#include <stdint.h>

#include "can.h"

#ifdef __cplusplus
#error "C++ is not supported"
#endif

void mcp2515_can_init(can_timing_t *can_params, uint8_t (*spi_read_fcn)(void),
					  void (*spi_write_fcn)(uint8_t data), void (*cs_drive_fcn)(uint8_t state));
void mcp2515_can_send(can_msg_t *msg);
bool mcp2515_can_send_rdy(void);
bool mcp2515_can_receive(can_msg_t *msg);

#endif
