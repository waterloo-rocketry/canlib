#ifndef _CANLIB_CANLIB_H
#define _CANLIB_CANLIB_H

#include "can.h"
#include "message_types.h"

#include "message/msg_actuator.h"
#include "message/msg_common.h"
#include "message/msg_general.h"
#include "message/msg_gps.h"
#include "message/msg_recovery.h"
#include "message/msg_sensor.h"
#include "message/msg_state_est.h"

#include "util/can_rcv_buffer.h"
#include "util/can_tx_buffer.h"
#include "util/timing_util.h"

#if (defined(STM32H733xx) || defined(STM32H750xx))

#include "stm32h7/stm32h7_can.h"

#elif ConfName == PIC18F26K83

#include "pic18f26k83/pic18f26k83_can.h"

#elif ConfName == MCP2515

#include "mcp2515/mcp_2515.h"

#elif ConfName == DSPIC33EPXXXGP50X

#include "dspic33epxxxgp50x/dspic33epxxxgp50x_can.h"

#else

#error "canlib project configuration not set."

#endif /* ConfName */

#endif /* CANLIB_H */
