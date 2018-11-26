#include "mcp_2515.h"

// FIXME: chip select pin handling

// SPI command macros
#define RESET       0b11000000
#define READ        0b00000011
#define READ_RX_B0  0b10010000  // read rx buffer 0
#define WRITE       0b00000010
#define LD_TX_B0    0b01000000  // read tx buffer 0
#define RTS_B0      0b10000000  // request to send on buff 0
#define READ_STAT   0b10100000
#define RX_STAT     0b10110000
#define BIT_MOD     0b00000101

// read a byte from the SPI module
static uint8_t (*spi_read)(void);

// write a byte through the spi module
static void (*spi_write)(uint8_t data);

// drive the chip select pin for the MCP2562
// 1: drive pin high, 0: drive pin low
static void (*cs_drive)(uint8_t state);

static void mcp_write_reg(uint8_t addr, uint8_t data) {
    cs_drive(0);
    spi_write(WRITE);
    spi_write(addr);
    spi_write(data);
    cs_drive(1);
}

static uint8_t mcp_read_reg(uint8_t addr) {
    cs_drive(0);
    spi_write(READ);
    spi_write(addr);
    uint8_t ret =  spi_read();
    cs_drive(1);
    return ret;
}

void mcp_can_init(can_timing_t *can_params,
                  uint8_t (*spi_read_fcn)(void),
                  void (*spi_write_fcn)(uint8_t data),
                  void (*cs_drive_fcn)(uint8_t state)) {
    spi_read = spi_read_fcn;
    spi_write= spi_write_fcn;
    cs_drive = cs_drive_fcn;

    cs_drive(1);

    // set to config mode. top 3 bits are 0b100
    mcp_write_reg(CANCTRL, 0x4 << 5);
    while (!(mcp_read_reg(CANCTRL))) { }

    mcp_write_reg(CNF1, can_params->sjw << 6 | can_params->brp);
    mcp_write_reg(CNF2, can_params->btlmode << 7 | can_params->sam << 6
        | can_params->seg1ph << 3 | can_params->prseg);
    mcp_write_reg(CNF3, can_params->seg2ph);

    // receive mode interrupts
    mcp_write_reg(RXB0CTRL, 0b0110000);
    mcp_write_reg(CANINTF, 0);      // fix later
    mcp_write_reg(CANINTE, 1);      // enable rxb0 interrupt
    mcp_write_reg(BFPCTRL, 0b101);      // set rxb0 interrupt output

    // set normal mode (top 3 bits = 0, set clock output)
    // set one shot mode
    mcp_write_reg(CANCTRL, 0xc);
    // mcp_write_reg(CANCTRL, 0x4);

    // normal mode: top 3 bits are 0
    while (mcp_read_reg(CANCTRL) & 0xe0 != 0);   // wait for normal mode
}

void mcp_can_send(can_msg_t *msg) {
    mcp_write_reg(CANINTF, 0);     // clear interrupt flag register
    mcp_write_reg(EFLG, 0);

    mcp_write_reg(TXB0SIDH, (uint8_t) (msg->sid >> 3));          // set sid
    mcp_write_reg(TXB0SIDL, (msg->sid & 0x7) << 5);              // set sid

    // data registers are consecutive
    for (int i = 0; i < msg->data_len; ++i) {
        mcp_write_reg(TXB0D0 + i, msg->data[i]);
    }

    // data message w/ some number of bytes
    mcp_write_reg(TXB0DLC, msg->data_len);
    mcp_write_reg(TXB0CTRL, 1 << 3);            // set txreq
}

void mcp_can_receive(can_msg_t *msg) {
    if (mcp_read_reg(CANINTF) & 0x1) {
        // rxb0 is full

        uint8_t sid_h = mcp_read_reg(RXB0SIDH);
        uint8_t sid_l = mcp_read_reg(RXB0SIDL);
        msg->sid = ((uint16_t)sid_h << 3) | sid_l >> 5;

        msg->data_len = mcp_read_reg(RXB0DLC) & 0xf;
        for (int i = 0; i < msg->data_len; ++i) {
            msg->data[i] = mcp_read_reg(RXB0D0 + i);
        }

    } else if (mcp_read_reg(CANINTF) & 0x2) {
        // rxb1 is full - lower priority
    }

    // should fix this in the future
    mcp_write_reg(CANINTF, 0);
}

