/*
 * File:   main.c
 * Author: alexm
 *
 * Created on July 17, 2018, 7:16 PM
 */

#define _XTAL_FREQ 4000000
#include "config.h"
#include "plib.h"
#include <stdint.h>

#define RESET       0b11000000
#define READ        0b00000011
#define READ_RX_B0  0b10010000  // read rx buffer 0
#define WRITE       0b00000010
#define LD_TX_B0    0b01000000  // read tx buffer 0
#define RTS_B0      0b10000000  // request to send on buff 0
#define READ_STAT   0b10100000
#define RX_STAT     0b10110000
#define BIT_MOD     0b00000101

#define LED_ON (LATD2 = 0)
#define LED_OFF (LATD2 = 1)

static void mcp_write_reg(uint8_t addr, uint8_t data) {
    LATD3 = 0;
    WriteSPI(WRITE);
    WriteSPI(addr);
    WriteSPI(data);
    LATD3 = 1;
}

static uint8_t mcp_read_reg(uint8_t addr) {
    LATD3 = 0;
    WriteSPI(READ);
    WriteSPI(addr);
    uint8_t ret =  ReadSPI();
    LATD3 = 1;
    return ret;
}

// CNF1 SJW = 0
// CNF1 BRP = 0b11111
static void can_init_spi() {
    TRISD3 = 0;
    LATD3 = 1;
    __delay_ms(100);    // yeah yeah
    
    // set to config mode. top 3 bits are 0b100
    mcp_write_reg(0xf, 0x4 << 5);
    while (!(mcp_read_reg(0xf))) {
        __delay_ms(5);
    }
    
    // hard code timing params for now until shit works
    
    // CNF1
    uint8_t sjw = 0b11;
    uint8_t brp = 0;
    mcp_write_reg(0x2a, sjw << 6 | brp);

    // CNF2
    uint8_t btlmode = 1;
    uint8_t sam = 0;
    uint8_t phseg1 = 0b100;
    uint8_t prseg1 = 0;
    mcp_write_reg(0x29, btlmode << 7 | sam << 6 | phseg1 << 3 | prseg1);
    
    // CNF3
    uint8_t phseg2 = 0b100;
    mcp_write_reg(0x28, phseg2);
    
    // set normal mode (top 3 bits = 0, set clock output)
    // set one shot mode - no retransmission
    mcp_write_reg(0xf, 0xc);

    // normal mode: top 3 bits are 0
    while (mcp_read_reg(0xf) & 0xe0 != 0);   // wait for normal mode
}

// quick and dirty and completely garbage
static void can_send(uint16_t sid) {
    mcp_write_reg(0x2c, 0);     // clear interrupt flag register
    mcp_write_reg(0x2d, 0);
    
    // 0x31 = txb0sidh
    // TODO stick all those in a header somewhere
    mcp_write_reg(0x31, (uint8_t) (sid >> 3));          // set sid
    mcp_write_reg(0x32, (sid & 0x7) << 5);  // set sid

    // data message w/ some number of bytes
    mcp_write_reg(0x35, 0);
    while (mcp_read_reg(0x35) != 0) {
        LED_ON;
        __delay_ms(100);
        LED_OFF;
        __delay_ms(100);
    }
    
    /*mcp_write_reg(0x37, 0xAA);*/
    /*mcp_write_reg(0x38, 0xAA);*/
    /*mcp_write_reg(0x39, 0xAA);*/
    /*mcp_write_reg(0x3A, 0xAA);*/
    /*mcp_write_reg(0x3b, 0xAA);*/
    /*mcp_write_reg(0x3c, 0xAA);*/
    /*mcp_write_reg(0x3d, 0xAA);*/
    mcp_write_reg(0x30, 1 << 3);            // set txreq
    
    mcp_read_reg(0x30);
    if (mcp_read_reg(0x2d)) {   // elfg
        LED_ON; // turn on if error
    } else {
        LED_OFF;
    }
}

void main(void) {
    // set up clock
    OSCCONbits.IRCF = 0x6;  // internal osc freq sel: 4 MHz
    OSCCONbits.SCS = 0x2;   // system clock sel
    
    // set an led for testing
    // led is on RD2
    TRISD2 = 0;     // output
    LED_OFF;

    // sync mode, bus mode, phase
    OpenSPI(SPI_FOSC_64, MODE_11, SMPMID);
   
    can_init_spi();
    mcp_read_reg(0x30);     // txb0ctrl
    mcp_read_reg(0x2d); // eflg

    while (1) {
        can_send(0x2aa);
        __delay_ms(200);
        
        can_send(0x444);
        __delay_ms(200);
    }
    
    while (1) {
        LATD2 = 0;
        __delay_ms(100);
        LATD2 = 1;
        __delay_ms(100);
    }
    
    return;
}
