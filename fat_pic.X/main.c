/*
 * File:   main.c
 * Author: alexm
 *
 * Created on July 17, 2018, 7:16 PM
 */

#define _XTAL_FREQ 4000000
#include "config.h"
#include "plib.h"

#define RESET       0b11000000
#define READ        0b00000011
#define READ_RX_B0  0b10010000  // read rx buffer 0
#define WRITE       0b00000010
#define LD_TX_B0    0b01000000  // read tx buffer 0
#define RTS_B0      0b10000000  // request to send on buff 0
#define READ_STAT   0b10100000
#define RX_STAT     0b10110000
#define BIT_MOD     0b00000101

// CNF1 SJW = 0
// CNF1 BRP = 0b11111
static void can_init_spi(char sjw, char brp) {
    TRISD3 = 0;
    LATD3 = 1;
    __delay_ms(100);    // yeah yeah
    
    LATD3 = 0;
    // set config mode first
    WriteSPI(WRITE);
    // what register is that even?
    WriteSPI(0xf);
    WriteSPI(0x4 << 5);
    LATD3 = 1;
    
    __delay_ms(10);
    LATD3 = 0;
    WriteSPI(READ);
    WriteSPI(0x0f); // canctrl
    char rip = ReadSPI();
    LATD3 = 1;
    
    __delay_ms(10);
    LATD3 = 0;
    char cmd = sjw << 5 | brp;
    WriteSPI(WRITE);    // write command
    WriteSPI(0x2a);     // cnf1 register addr
    WriteSPI(cmd);      // set the register
    LATD3 = 1;
    
    __delay_ms(10);
    LATD3 = 0;
    WriteSPI(WRITE);
    WriteSPI(0x0f);     // can ctrl
    WriteSPI(0x7);      // normal mode with clock out
    LATD3 = 1;
}

// quick and dirty and completely garbage
static void can_send() {
    
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
    while (1) {
        LATD2 = 0;
        __delay_ms(100);
        LATD2 = 1;
        __delay_ms(100);
    }
    
    // try to just write for a while
//    while (1) {
//        WriteSPI(RTS_B0);
//    }
//    
    return;
}
