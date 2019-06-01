#include "../../dspic33epxxxgp50x/dspic33epxxxgp50x_can.h"

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = ON              // Watchdog Timer Enable bit (Watchdog timer always enabled)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FOSCSEL
#pragma config FNOSC = FRCDIVN          // Oscillator Source Selection (Internal Fast RC (FRC) Oscillator with postscaler)
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <libpic30.h>


#define LED_1_ON()  (TRISBbits.TRISB13 = 0)
#define LED_1_OFF() (TRISBbits.TRISB13 = 1)

#define LED_2_ON()  (TRISBbits.TRISB14 = 0)
#define LED_2_OFF() (TRISBbits.TRISB14 = 1)

#define SID_LED_1 0x7EF
#define SID_LED_2 0x7EA

// This function runs in an interrupt context, so don't put any delays
// or any heavy algorithms in it
void can_callback_function(const can_msg_t *message) {
    if(message->sid == SID_LED_1) {
        LED_1_ON();
        LED_2_OFF();
    } else if (message->sid == SID_LED_2){
        LED_1_OFF();
        LED_2_ON();
    } else {
        LED_1_OFF();
        LED_2_OFF();
    }
}

int main() {
    // initialize LEDs
    LED_1_OFF();
    LED_2_OFF();
    LATBbits.LATB13 = 0;
    LATBbits.LATB14 = 0;

    // setup CAN timing - all values are random
    can_timing_t timing;
    timing.brp = 0x3f;
    timing.sjw = 3;
    timing.btlmode = 1;
    timing.prseg = 3;
    timing.sam = 0;
    timing.seg1ph = 0;
    timing.seg2ph = 0;
    init_can(&timing, can_callback_function, true);

    // create 2 CAN messages: 1 turns on LED1, the other turns on LED2
    can_msg_t led1, led2;
    led1.data_len = 3;
    led1.sid = SID_LED_1;
    led1.data[0] = 0xAA;
    led1.data[1] = 0xBB;
    led1.data[2] = 0xCC;
    led2.data_len = 3;
    led2.sid = SID_LED_2;
    led1.data[0] = 0xDD;
    led1.data[1] = 0xEE;
    led1.data[2] = 0xFF;

    // send 1 then the other forever
    while(1) {
        can_send(&led1);
        // this delay feels like 200ms maybe?
        __delay32(1000000);
        can_send(&led2);
        __delay32(1000000);
    }

}
