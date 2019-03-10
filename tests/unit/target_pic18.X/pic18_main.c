// PIC18F26K83 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config FEXTOSC = OFF    // External Oscillator Selection (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_1MHZ// Reset Oscillator Selection (HFINTOSC with HFFRQ = 4 MHz and CDIV = 4:1)

// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON      // PRLOCKED One-Way Set Enable bit (PRLOCK bit can be cleared and set only once)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCK bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_2P45 // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG4H
#pragma config WRTB = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-30000Bh) not write-protected)
#pragma config WRTC = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

// CONFIG5H

#include <xc.h>
#include <stdbool.h>
#include "build_can_message.h"
#include "can_common_tests.h"
#include "can_buffering_layer.h"

// These LEDs are the ones that exist on picdev. So run these tests
// on radio picdev
static void leds_init()
{
    // set all three leds as outputs
    LATB4 = 1;
    TRISB4 = 0;
    LATB3 = 1;
    TRISB3 = 0;
    LATB2 = 1;
    TRISB2 = 0;
}

#define LED1_ON() (LATB4 = 0)
#define LED1_OFF() (LATB4 = 1)
#define LED2_ON() (LATB3 = 0)
#define LED2_OFF() (LATB3 = 1)
#define LED3_ON() (LATB2 = 0)
#define LED3_OFF() (LATB2 = 1)

int main()
{
    // initialization stuff
    leds_init();

    bool all_tests_passed = true;
    // while we're running the tests, turn LED 2 on
    LED2_ON();

    if (!test_build_can_message()) {
        all_tests_passed = false;
    }
    if (!test_can_common_functions()) {
        all_tests_passed = false;
    }
    if (!test_debug_macro()) {
        all_tests_passed = false;
    }
    if (!test_can_buffering_layer()) {
        all_tests_passed = false;
    }

    // end of the tests, turn LED 2 off
    LED2_OFF();
    if (all_tests_passed) {
        // the success LED is LED 3
        LED3_ON();
    } else {
        // the failure LED is LED 1
        LED1_ON();
    }

    // loop forever
    for (;;) {}
    return 0;
}

