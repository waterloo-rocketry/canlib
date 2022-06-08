#ifndef CANLIB_H
#define	CANLIB_H

#include "can.h"
#include "can_common.h"
#include "message_types.h"

#include "util/can_rcv_buffer.h"
#include "util/can_tx_buffer.h"
#include "util/timing_util.h"

#if ConfName == PIC18F26K83

#include "pic18f26k83/pic18f26k83_can.h"
#include "pic18f26k83/pic18f26k83_timer.h"

#elif ConfName == MCP2515

#include "mcp2515/mcp_2515.h"

#elif ConfName == DSPIC33EPXXXGP50X

#include "dspic33epxxxgp50x/dspic33epxxxgp50x_can.h"

#else

#error "canlib project configuration not set."

#endif /* ConfName */

#endif	/* CANLIB_H */

