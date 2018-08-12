#define _XTAL_FREQ 1000000
#include "../inc/config.h"  // REEEEEEEEEE
#include <stdint.h>

static void can_init() {
    // select CAN pins
    CANRXPPS = 0x11;    // CAN receive pin = RC1
    RC0PPS = 0x33;      // CAN transmit pin = RC0

    // init gpio pins
    // tx
    TRISC0 = 0; // out
    LATC0 = 1;
    ODCC0 = 1;
    ANSELC0 = 0;

    // rx
    TRISC1 = 1; // in
    ANSELC1 = 0;
    
    // set can config mode
    CANCONbits.REQOP = 0b100;
    while (CANSTATbits.OPMODE != 0x4);

    // select functional mode
    // L E G A C Y
    ECANCONbits.MDSEL = 0;

    // set baud rate
    CIOCONbits.CLKSEL = 0;
    BRGCON1bits.SJW = 0b11;
    BRGCON1bits.BRP = 0x0;
    
    // these probably all default to 0 anyway?
    BRGCON2bits.SEG2PHTS = 1;
    BRGCON2bits.SAM = 0;
    BRGCON2bits.SEG1PH = 0b100;
    BRGCON2bits.PRSEG = 0b010;

    BRGCON3bits.WAKDIS = 1; // we'll eventually want this but not now
    BRGCON3bits.WAKFIL = 0;
    BRGCON3bits.SEG2PH2 = 0b100;
    
    // set filters and masks
    // masks
    // we don't give a fuck about filters
    RXM0SIDH = 0;
    RXM0SIDL = 0;
    RXM1SIDH = 0;
    RXM1SIDL = 0;
    
    // accept all messages for now
    RXB0CON = 0x60;

    // set loopback
    //CANCONbits.REQOP = 0x2;
    
    // set normal mode
    CANCONbits.REQOP = 0;
    while (CANSTATbits.OPMODE != 0x0);

    // deal with interrupt shit
    CANSTATbits.ICODE = 0;
    PIE5bits.IRXIE = 1; // invalid massage
    PIE5bits.WAKIE = 0;
    PIE5bits.ERRIE = 1;

    PIE5bits.TXB2IE = 1;
    PIE5bits.TXB1IE = 1;
    PIE5bits.TXB0IE = 1;

    PIE5bits.RXB1IE = 1;
    PIE5bits.RXB0IE = 1;

    // fuck
}

static void can_send(uint16_t sid) {
    if (TXB0CONbits.TXREQ != 0) {
        return;
    }

    TXB0CONbits.TXPRI = 0; // lowest priority
    TXB0SIDH = (sid >> 3);
    TXB0SIDL = ((sid & 0x7) << 5);
    TXB0DLCbits.TXRTR = 0;  //not an RTR, whatever that means
    TXB0DLCbits.DLC = 0;    // send no bytes, just sid

    // politely request a cordial transmission to the ether
    TXB0CONbits.TXREQ = 1;
}

static void LED_init() {
    //first LED is on RC2
    TRISC2 = 0; //output
    LATC2 = 1; //turn it off

    //second LED is on RC3
    TRISC3 = 0; //output
    LATC3 = 1; //turn off
    
    TRISC4 = 0;
    LATC4 = 1;
}

#define LED_1_ON (LATC2 = 0)
#define LED_1_OFF (LATC2 = 1)
#define LED_2_ON (LATC3 = 0)
#define LED_2_OFF (LATC3 = 1)
#define LED_3_ON (LATC4 = 0)
#define LED_3_OFF (LATC4 = 1)

static void interrupt fuck_everything() {
    
    if (PIR5bits.TXB0IF) {
        PIR5bits.TXB0IF = 0;
        return;
    }
    
    if (PIR5bits.RXB0IF || PIR5bits.RXB1IF) {
        uint16_t sid = (((uint16_t)RXB0SIDH) << 3) | (RXB0SIDL >> 5);
        RXB0CONbits.RXFUL = 0;
        if (sid == 0x2aa) {
            LED_1_OFF;
            LED_2_ON;
        } else if (sid == 0x444) {
            LED_1_ON;
            LED_2_OFF;
        } else {
            while(1) {
                LED_1_ON;
                __delay_ms(10);
                LED_1_OFF;
                __delay_ms(10);
            }
        }

        PIR5bits.RXB0IF = 0;
        PIR5bits.RXB1IF = 0;
        return;
    }
    
    if (PIR5bits.IRXIF) {
        PIR5bits.IRXIF = 0;
        return;
    }
    
    if (PIR5bits.ERRIF) {
        PIR5bits.ERRIF = 0;
        return;
    }

    PIR5;
    COMSTAT;
    while (1);
}


void main(void) {
        
    // enable global peripheral interrupts
    INTCON0bits.IPEN = 0;
    INTCON0bits.GIE = 1;

    can_init();
    LED_init();

    while (1) {
        //turn on LEDs sid
        if (COMSTAT) {
            LED_3_ON;
        } else {
            LED_3_OFF;
        }
        RXERRCNT;
        /*
        can_send(0x1);
        __delay_ms(100);
        
        COMSTAT;
        TXB0CON;
        PIR5;
        //turn off LEDs sid
        can_send(0x2);
        __delay_ms(100);
         */
    }
}

