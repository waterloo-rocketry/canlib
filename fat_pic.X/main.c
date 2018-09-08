#define _XTAL_FREQ 4000000

#include "../mcp2515/mcp_2515.h"    // I have stared into darkness
#include "config.h"
#include "plib.h"
#include <stdint.h>
#include <string.h>

#define LED_1_ON (LATD1 = 0)
#define LED_1_OFF (LATD1 = 1)
#define LED_2_ON (LATD2 = 0)
#define LED_2_OFF (LATD2 = 1)

// CAN receive buffer for testing purposes
static can_msg_t rcv_can;

static void interrupt rcv_test() {
    LED_1_ON;
    __delay_ms(100);
    LED_1_OFF;

    if (INTCON3bits.INT2IF) { 
        mcp_can_receive(&rcv_can);
        INTCON3bits.INT2IF = 0;
    }
}

void main(void) {
    // set up clock
    OSCCONbits.IRCF = 0x6;  // internal osc freq sel: 4 MHz
    OSCCONbits.SCS = 0x2;   // system clock sel

    // set up LEDs
    TRISD1 = 0;
    TRISD2 = 0;
    LED_1_OFF;
    LED_2_OFF;
    
    // sync mode, bus mode, phase
    OpenSPI(SPI_FOSC_64, MODE_11, SMPMID);
   
    // XC8 doesn't support designated initializers
    can_t can_params;
    can_params.brp = 0;               // tq = 2 us
    can_params.sjw = 0x3;             // sjw = 4 tq
    can_params.btlmode = 0x1;         // ph2seg freely programmable
    can_params.sam = 0;               // sample once per bit
    can_params.seg1ph = 0x4;          // tseg1 = 5 tq
    can_params.prseg1 = 0x0;          // tprseg1 = 1 tq
    can_params.seg2ph = 0x4;          // tseg2 = 5 tq
    mcp_can_init(&can_params, ReadSPI, WriteSPI);
    
    // set up interrupts - these are the responsibility of the MCU code, not canlib
    RCONbits.IPEN = 0;      // disable priority based interrupts
    INTCON3bits.INT2IF = 0;
    INTCON2bits.INTEDG2 = 0; // interrupt on falling edge
    INTCON3bits.INT2IE = 1;
    INTCONbits.GIE = 1;     // enable global interrupts

    uint8_t data_0[] = {0xca, 0xfe, 0xba, 0xbe};
    uint8_t data_1[] = {0xde, 0xad, 0xbe, 0xef};
    
    can_msg_t msg0;
    memcpy(msg0.data, data_0, sizeof(data_0));
    msg0.sid = 0x2aa;
    msg0.data_len = 4;
    
    can_msg_t msg1;
    memcpy(msg1.data, data_1, sizeof(data_1));
    msg1.sid = 0x444;
    msg1.data_len = 4;
    
    while (1) {
        /*
        mcp_can_send(&msg0);
        __delay_ms(1000);
        mcp_can_send(&msg1);
        __delay_ms(1000);
       */ 
    }
    return;
}
