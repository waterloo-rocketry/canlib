/*
 * File:   main.c
 * Author: alexm
 *
 * Created on July 17, 2018, 7:16 PM
 */

#define _XTAL_FREQ 4000000
#include "config.h"
#include "plib.h"
#include "mcp_2515.h"
#include <stdint.h>

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
    
    can_init_spi(0, 0x3f);
    
    while (1) {
        can_send(0x1);
        mcp_read_reg(0x30);     // txb0ctrl
        mcp_read_reg(0x2d);     // eflg
        
        __delay_ms(500);
        can_send(0x2);
        __delay_ms(500);      
    }

    return;
}
