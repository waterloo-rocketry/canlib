#ifndef MCP_2515_H_
#define MCP_2515_H_

#include "../can.h"

void mcp_can_init(can_timing_t *can_params,
                  uint8_t (*spi_read_fcn)(void),
                  void (*spi_write_fcn)(uint8_t data),
                  void (*cs_drive_fcn)(uint8_t state));
void mcp_can_send(can_msg_t *msg);
bool mcp_can_send_rdy(void);
bool mcp_can_receive(can_msg_t *msg);

// register addresses
#define RXF0SIDH  0x00
#define RXF3SIDH  0x10
#define RXM0SIDH  0x20
#define TXB0CTRL  0x30
#define TXB1CTRL  0x40
#define TXB2CTRL  0x50
#define RXB0CTRL  0x60
#define RXB1CTRL  0x70

#define RXF0SIDL  0x01
#define RXF3SIDL  0x11
#define RXM0SIDL  0x21
#define TXB0SIDH  0x31
#define TXB1SIDH  0x41
#define TXB2SIDH  0x51
#define RXB0SIDH  0x61
#define RXB1SIDH  0x71

#define RXF0EID8  0x02
#define RXF3EID8  0x12
#define RXM0EID8  0x22
#define TXB0SIDL  0x32
#define TXB1SIDL  0x42
#define TXB2SIDL  0x52
#define RXB0SIDL  0x62
#define RXB1SIDL  0x72

#define RXF0EID0  0x03
#define RXF3EID0  0x13
#define RXM0EID0  0x23
#define TXB0EID8  0x33
#define TXB1EID8  0x43
#define TXB2EID8  0x53
#define RXB0EID8  0x63
#define RXB1EID8  0x73

#define RXF1SIDH  0x04
#define RXF4SIDH  0x14
#define RXM1SIDH  0x24
#define TXB0EID0  0x34
#define TXB1EID0  0x44
#define TXB2EID0  0x54
#define RXB0EID0  0x64
#define RXB1EID0  0x74

#define RXF1SIDL  0x05
#define RXF4SIDL  0x15
#define RXM1SIDL  0x25
#define TXB0DLC   0x35
#define TXB1DLC   0x45
#define TXB2DLC   0x55
#define RXB0DLC   0x65
#define RXB1DLC   0x75

#define RXF1EID8  0x06
#define RXF4EID8  0x16
#define RXM1EID8  0x26
#define TXB0D0    0x36
#define TXB1D0    0x46
#define TXB2D0    0x56
#define RXB0D0    0x66
#define RXB1D0    0x76

#define RXF1EID0  0x07
#define RXF4EID0  0x17
#define RXM1EID0  0x27
#define TXB0D1    0x37
#define TXB1D1    0x47
#define TXB2D1    0x57
#define RXB0D1    0x67
#define RXB1D1    0x77

#define RXF2SIDH  0x08
#define RXF5SIDH  0x18
#define CNF3      0x28
#define TXB0D2    0x38
#define TXB1D2    0x48
#define TXB2D2    0x58
#define RXB0D2    0x68
#define RXB1D2    0x78

#define RXF2SIDL  0x09
#define RXF5SIDL  0x19
#define CNF2      0x29
#define TXB0D3    0x39
#define TXB1D3    0x49
#define TXB2D3    0x59
#define RXB0D3    0x69
#define RXB1D3    0x79

#define RXF2EID8  0x0A
#define RXF5EID8  0x1A
#define CNF1      0x2A
#define TXB0D4    0x3A
#define TXB1D4    0x4A
#define TXB2D4    0x5A
#define RXB0D4    0x6A
#define RXB1D4    0x7A

#define RXF2EID0  0x0B
#define RXF5EID0  0x1B
#define CANINTE   0x2B
#define TXB0D5    0x3B
#define TXB1D5    0x4B
#define TXB2D5    0x5B
#define RXB0D5    0x6B
#define RXB1D5    0x7B

#define BFPCTRL   0x0C
#define TEC       0x1C
#define CANINTF   0x2C
#define TXB0D6    0x3C
#define TXB1D6    0x4C
#define TXB2D6    0x5C
#define RXB0D6    0x6C
#define RXB1D6    0x7C

#define TXRTSCTRL 0x0D
#define REC       0x1D
#define EFLG      0x2D
#define TXB0D7    0x3D
#define TXB1D7    0x4D
#define TXB2D7    0x5D
#define RXB0D7    0x6D
#define RXB1D7    0x7D

#define CANSTAT   0x0E
#define CANCTRL   0x0F

#endif // MCP_2515_H_
