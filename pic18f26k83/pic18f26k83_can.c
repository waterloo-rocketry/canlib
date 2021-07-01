#include "pic18f26k83_can.h"
#include <xc.h>
#include <string.h>

static void (*can_rcv_cb)(const can_msg_t *message);

/*
 * Initialize the CAN driver on a PIC18F26fk83. Note that this function
 * DOES NOT setup the inputs and outputs from the CAN module to the
 * output pins, application code must do that. In order to do that,
 * CANRXPPS must be set to the proper pin value for the CANRX pin, and
 * ___PPS must be set to 0x33 to mark it as outputting from the CAN
 * module. In addition, TRIS and ANSEL registers for whatever pin
 * is being used must be set to the right values.
 */
void can_init(const can_timing_t *timing,
              void (*receive_callback)(const can_msg_t *message)) {
    //keep track of callback, we use it in interrupts
    can_rcv_cb = receive_callback;

    // set can module to config mode
    CANCONbits.REQOP = 0b100;
    // wait until that mode takes effect
    while (CANSTATbits.OPMODE != 0x4);

    // put the can module into legacy mode, which is easier to work
    // with
    ECANCONbits.MDSEL = 0;

    // set baud rate
    // this sets the CAN module to run off of the system clock
    CIOCONbits.CLKSEL = 0;

    BRGCON1bits.SJW = timing->sjw;
    BRGCON1bits.BRP = timing->brp;
    BRGCON2bits.SEG2PHTS = timing->btlmode;
    BRGCON2bits.SAM = timing->sam;
    BRGCON2bits.SEG1PH = timing->seg1ph;
    BRGCON2bits.PRSEG = timing->prseg;
    BRGCON3bits.SEG2PH = timing->seg2ph;

    // the can module has the ability to wake the microcontroller up
    // from sleep when it sees activity on the bus. To enable this
    // feature, set WAKDIS bit to 0. Driver does not currently support
    // this.
    BRGCON3bits.WAKDIS = 1; // we'll eventually want this but not now
    BRGCON3bits.WAKFIL = 0;

    // This driver does not currently support hardware filtering of
    // incoming messages
    RXM0SIDH = 0;
    RXM0SIDL = 0;
    RXM1SIDH = 0;
    RXM1SIDL = 0;

    //ignore all receive message mask behaviour
    // ignore all receive message mask behaviour
    RXB0CONbits.RXM0 = 1;
    RXB0CONbits.RXM1 = 1;

    // Allow overflowing of messages into RXBUF1 if RXBUF0 is full
    RXB0CONbits.RB0DBEN = 1;

    // enable interrupts for all useful CAN interrupts
    //  interrupt triggered on invalid message received
    PIE5bits.IRXIE = 1;
    //  disable wakeup interrupts
    PIE5bits.WAKIE = 0;
    //  CAN module error interrupt
    PIE5bits.ERRIE = 1;
    //  interrupt not generated whenever a CAN message is sent
    PIE5bits.TXB2IE = 0;
    PIE5bits.TXB1IE = 0;
    PIE5bits.TXB0IE = 0;
    //  interrupt generated everytime that a CAN message is received
    PIE5bits.RXB1IE = 1;
    PIE5bits.RXB0IE = 1;

    // set normal mode
    CANCONbits.REQOP = 0;
    // wait until change is applied
    while (CANSTATbits.OPMODE != 0x0);
}

void can_send(const can_msg_t* message) {
    // at present, this fails if transmit buffer 0 isn't available
    if (TXB0CONbits.TXREQ != 0) {
        return;
    }

    // argument checking
    if(message->data_len > 8 || message->sid > 0x7FF) {
        return;
    }

    // All messages are the highest priority
    TXB0CONbits.TXPRI = 0;
    TXB0SIDH = ((message->sid) >> 3);
    TXB0SIDL = (((message->sid) & 0x7) << 5);

    // not an RTR message, we don't support those
    TXB0DLCbits.TXRTR = 0;
    // set message length
    TXB0DLCbits.DLC = message->data_len;

    // copy data over. TXB0D1 is immediately after TXB0D0, which is why
    // this is legal
    memcpy((void*) &TXB0D0, message->data, message->data_len);

    // Mark transmit buffer 0 ready to transmit
    TXB0CONbits.TXREQ = 1;
}

bool can_send_rdy(void) {
    return TXB0CONbits.TXREQ == 0;
}

// if any bit is set in PIR5 during an interrupt service routine, call
// this funtion, and it will handle it
void can_handle_interrupt() {
    // if there was already a message in the receive buffer and we
    // received another message, just drop that message. Apparently
    // your code isn't fast enough.
    if (COMSTATbits.RXB0OVFL || COMSTATbits.RXB1OVFL) {
        COMSTATbits.RXB0OVFL = 0;
        COMSTATbits.RXB1OVFL = 0;
    }

    // handle a received message by stuffing it into a can_message_t
    // and calling the application code provided callback
    if (PIR5bits.RXB0IF) {
        can_msg_t rcvd_msg;
        rcvd_msg.sid = (((uint16_t)RXB0SIDH) << 3) | (RXB0SIDL >> 5);
        rcvd_msg.data_len = RXB0DLCbits.DLC;
        memcpy(rcvd_msg.data, (const void *) &RXB0D0, rcvd_msg.data_len);

        // call application code callback
        if (NULL != can_rcv_cb) {
            can_rcv_cb(&rcvd_msg);
        }

        PIR5bits.RXB0IF = 0;
        RXB0CONbits.RXFUL = 0;
        return;
    } else if (PIR5bits.RXB1IF) {

        can_msg_t rcvd_msg;
        rcvd_msg.sid = (((uint16_t)RXB1SIDH) << 3) | (RXB1SIDL >> 5);
        rcvd_msg.data_len = RXB1DLCbits.DLC;
        memcpy(rcvd_msg.data, (const void *) &RXB1D0, rcvd_msg.data_len);

        // call application code callback
        if (NULL != can_rcv_cb) {
            can_rcv_cb(&rcvd_msg);
        }

        PIR5bits.RXB1IF = 0;
        RXB1CONbits.RXFUL = 0;
    }

    // A message was transmitted. We don't currently care about this,
    // so ignore it
    else if (PIR5bits.TXB0IF) {
        PIR5bits.TXB0IF = 0;
        return;
    } else if (PIR5bits.TXB1IF) {
        PIR5bits.TXB1IF = 0;
        return;
    } else if (PIR5bits.TXB2IF) {
        PIR5bits.TXB2IF = 0;
        return;
    } else if (PIR5bits.IRXIF) {
        // I don't actually know how to handle an invalid message
        // receive. So we don't handle it.
        PIR5bits.IRXIF = 0;
        return;
    } else if (PIR5bits.ERRIF) {
        // No idea how to handle an error. So ignore it, what's the
        // worst that can happen?
        PIR5bits.ERRIF = 0;
        return;
    }
}
