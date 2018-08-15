#include "mcp_2515.h"
#include "plib.h"

//SPI command macros
#define RESET       0b11000000
#define READ        0b00000011
#define READ_RX_B0  0b10010000  // read rx buffer 0
#define WRITE       0b00000010
#define LD_TX_B0    0b01000000  // read tx buffer 0
#define RTS_B0      0b10000000  // request to send on buff 0
#define READ_STAT   0b10100000
#define RX_STAT     0b10110000
#define BIT_MOD     0b00000101

void mcp_init() {
    // sync mode, bus mode, phase
    OpenSPI(SPI_FOSC_64, MODE_11, SMPMID);
}

void mcp_write_reg(uint8_t addr, uint8_t data) {
    LATD3 = 0;
    WriteSPI(WRITE);
    WriteSPI(addr);
    WriteSPI(data);
    LATD3 = 1;
}

uint8_t mcp_read_reg(uint8_t addr) {
    LATD3 = 0;
    WriteSPI(READ);
    WriteSPI(addr);
    uint8_t ret =  ReadSPI();
    LATD3 = 1;
    return ret;
}

