#define _XTAL_FREQ 1000000
#include "../inc/config.h"  // REEEEEEEEEE

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
    while (CANSTATbits.OPMODE & 0x7 != 0x4);

    // select functional mode
    // L E G A C Y
    ECANCONbits.MDSEL = 0;

    // set baud rate
    CIOCONbits.CLKSEL = 0;
    BRGCON1bits.SJW = 0;
    BRGCON1bits.BRP = 0x3f;   // burp big is 1 khz

    BRGCON2bits.SEG2PHTS = 0;
    BRGCON2bits.SAM = 0;
    BRGCON2bits.SEG1PH = 0;
    BRGCON2bits.PRSEG = 0;

    BRGCON3bits.WAKDIS = 1; // we'll eventually want this but not now
    BRGCON3bits.WAKFIL = 0;
    BRGCON3bits.SEG2PH2 = 0;
    
    // set filters and masks
    // masks
    // we don't give a fuck about filters
    RXM0SIDH = 0;
    RXM0SIDL = 0;
    RXM1SIDH = 0;
    RXM1SIDL = 0;

    // set loopback
    CANCONbits.REQOP = 0x2;
    while (CANSTATbits.OPMODE & 0x7 != 0x2);

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

static void can_send() {
    if (TXB0CONbits.TXREQ != 0) {
        return;
    }

    TXB0CONbits.TXPRI = 0; // lowest priority
    TXB0SIDH = 0xaa;
    TXB0SIDL = 0xe0;
    TXB0DLCbits.TXRTR = 0;
    TXB0DLCbits.DLC = 4;    // send 4 bytes

    // aaron is 12
    TXB0D0 = 0xb1;
    TXB0D1 = 0x6b;
    TXB0D2 = 0x00;
    TXB0D3 = 0xb5;

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
}

#define LED_1_ON (PORTC2 = 0)
#define LED_1_OFF (PORTC2 = 1)
#define LED_2_ON (PORTC3 = 0)
#define LED_2_OFF (PORTC3 = 1)

static void interrupt fuck_everything() {
    while (1);
    PIR5;
/*    RXB0CON;*/
    /*RXB0SIDL;*/
    /*RXB0SIDH;*/
    /*RXB0DLC;*/
    /*RXB0D0;*/
    /*RXB0D1;*/
    /*RXB0D2;*/
/*    RXB0D3;*/
}

void main(void) {
        
    // enable global peripheral interrupts
    INTCON0bits.IPEN = 0;
    INTCON0bits.GIE = 1;

    can_init();
    LED_init();

    __delay_ms(2000);
    while (1) {
        can_send();
        __delay_ms(100);
    }
}

