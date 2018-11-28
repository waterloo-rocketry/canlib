#include "spi.h"
#include <stdint.h>
#include <xc.h>

void spi_init() {
    //mark SCL as output
    TRISCbits.TRISC0 = 0;
    //mark MISO as input
    TRISCbits.TRISC1 = 1;
    //mark MOSI as output
    TRISCbits.TRISC2 = 0;

    //bits 0->2 of ANSELC need to be low
    ANSELC &= ~(0x07);

    SSPSTAT = 0b10000000;
    SSPCON1 = 0b00110010;
    SSPCON2 = 0b00000000;
    
}

void spi_write(uint8_t data) {
    uint8_t temp = SSPBUF;
    PIR1bits.SSP1IF = 0;
    SSPCONbits.WCOL = 0;
    SSPBUF = data;
    while(!PIR1bits.SSP1IF) {};
}

uint8_t spi_read(void) {
    //read the SSPBUF so it doesn't yell at you
    uint8_t temp = SSPBUF;
    PIR1bits.SSP1IF = 0;
    //clock 0 onto the pins
    SSPBUF = 0;
    while(!PIR1bits.SSP1IF) {}
    return SSPBUF;
}

void cs_drive(uint8_t state) {
    if(state)
        LATC |= (1 << 3);
    else
        LATC &= ~(1 << 3);
}
