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
static void can_init_spi(char sjw, char brp) {
    TRISD3 = 0;
    LATD3 = 1;
    __delay_ms(100);    // yeah yeah
    
    mcp_write_reg(0xf, 0x4 << 5 | 7);
    while (!(mcp_read_reg(0xf))) {
        __delay_ms(5);
    }

    __delay_ms(10);
    uint8_t cmd = sjw << 5 | brp;
    mcp_write_reg(0x2a, cmd);
    
    // set up filters and masks
    /*
    mcp_write_reg(0x28, 1);
    mcp_write_reg(0x0, 0xff);
    mcp_write_reg(0x1, 0xe0);
    mcp_write_reg(0x4, 0xff);
    mcp_write_reg(0x5, 0xe0);
    mcp_write_reg(0x8, 0xff);
    mcp_write_reg(0x9, 0xe0);
    mcp_write_reg(0x10, 0xff);
    mcp_write_reg(0x11, 0xe0);
    mcp_write_reg(0x14, 0xff);
    mcp_write_reg(0x15, 0xe0);
    mcp_write_reg(0x18, 0xff);
    mcp_write_reg(0x19, 0xe0);
    
    // debug
    mcp_read_reg(0x30);
    mcp_read_reg(0x2d);
    
    mcp_write_reg(0x20, 0xff);
    mcp_write_reg(0x24, 0xff);
    
    // debug
    mcp_read_reg(0x30);
    mcp_read_reg(0x2d);
     */
    
    mcp_read_reg(0x30);     // txb0ctrl
    mcp_read_reg(0x2d); 
    
    //mcp_write_reg(0xf, 0b01000110);     // set loopback
    mcp_write_reg(0xf, 0x7);
    while (!(mcp_read_reg(0xf) & 0x7));   // wait for normal mode
}

// quick and dirty and completely garbage
static void can_send(uint16_t sid) {
    mcp_write_reg(0x2c, 0);     // clear interrupt flag register
    mcp_write_reg(0x2d, 0);
    mcp_write_reg(0x20, sid >> 3);          // set sid
    mcp_write_reg(0x21, (sid & 0x7) << 5);  // set sid
    mcp_write_reg(0x35, 0);                 // length = 0, data message
    mcp_write_reg(0x30, 1 << 3);            // set txreq
    mcp_read_reg(0x30);
    mcp_read_reg(0x2d);
}


void main(void) {
    // set up clock
    OSCCONbits.IRCF = 0x6;  // internal osc freq sel: 4 MHz
    OSCCONbits.SCS = 0x2;   // system clock sel
    
    // set an led for testing
    // led is on RD2
    TRISD2 = 0;     // output
    LATD2 = 1;

    // sync mode, bus mode, phase
    OpenSPI(SPI_FOSC_64, MODE_11, SMPMID);
    
    can_init_spi(0, 0x3f);
    mcp_read_reg(0x30);     // txb0ctrl
    mcp_read_reg(0x2d);     // eflg
    
    /*
    while (1) {
        can_send(0x1);
        mcp_read_reg(0x30);     // txb0ctrl
        mcp_read_reg(0x2d);     // eflg
        
        __delay_ms(500);
        can_send(0x2);
        __delay_ms(500);      
    }
    
    while (1) {
        LATD2 = 0;
        __delay_ms(100);
        LATD2 = 1;
        __delay_ms(100);
    }
     */
    
    // try to just write for a while
//    while (1) {
//        WriteSPI(RTS_B0);
//    }
//    
    return;
}
