#include "dspic33epxxxgp50x_can.h"
#include <xc.h>

static void (*can_rcv_cb)(const can_msg_t *message);

// make a RAM buffer big enough to hold 32 CAN message
#define CAN_BUF_NUM 32
static unsigned int can_msg_buf[CAN_BUF_NUM][8] __attribute__((aligned(CAN_BUF_NUM * 16)));

// private functions
static void init_filters_and_masks();
static void init_dma_channels();
static void init_can_interrupts();
static void buffer_to_msg(const unsigned int *buffer, can_msg_t *received);

// Initialization function for CAN. The driver currently only runs in
// callback mode, and that callback will be triggered in an interrupt
// context. Please make the callback as short (in time) as possible.
void init_can(const can_timing_t *timing,
              void (*receive_callback)(const can_msg_t *message),
              bool run_in_loopback) {
    // store pointer to the receive callback
    can_rcv_cb = receive_callback;

    // set can module to config mode
    C1CTRL1bits.REQOP = 0x4;
    // wait until that mode takes effect
    while (C1CTRL1bits.OPMODE != 0x4);

    // allow writing of control bits
    C1CTRL1bits.WIN = 1;

    // set baud rate and other timing details
    C1CFG1bits.SJW = timing->sjw;
    C1CFG1bits.BRP = timing->brp;
    C1CFG2bits.SEG2PHTS = timing->btlmode;
    C1CFG2bits.SAM = timing->sam;
    C1CFG2bits.SEG1PH = timing->seg1ph;
    C1CFG2bits.PRSEG = timing->prseg;
    C1CFG2bits.SEG2PH = timing->seg2ph;

    // set up DMA and FIFO: don't use the FIFO, use 32 buffers for DMA
    C1FCTRLbits.DMABS = 0b110;
    C1FCTRLbits.FSA   = 0b11111;

    // set up filters and masks
    init_filters_and_masks();

    // disable writes to C1CTRL
    C1CTRL1bits.WIN = 0;

    // set the first transmit buffer as transmit, all others as
    // receive buffers (we don't need plural transmits (I think))
    C1TR01CONbits.TXEN0 = 0x1;
    C1TR01CONbits.TXEN1 = 0x0;
    C1TR23CONbits.TXEN2 = 0x0;
    C1TR23CONbits.TXEN3 = 0x0;
    C1TR45CONbits.TXEN4 = 0x0;
    C1TR45CONbits.TXEN5 = 0x0;
    C1TR67CONbits.TXEN6 = 0x0;
    C1TR67CONbits.TXEN7 = 0x0;

    // initialize the DMA module: This driver requires DMA0 and DMA1,
    // so don't use those elsewhere
    init_dma_channels();

    // if you wanna run this driver in loopback mode, we can
    // accomodate that
    if(run_in_loopback) {
        // set loopback mode
        C1CTRL1bits.REQOP = 2;
        // wait until change is applied
        while (C1CTRL1bits.OPMODE != 0x2);
    } else {
        C1CTRL1bits.REQOP = 0;
        while(C1CTRL1bits.OPMODE != 0);
    }

    // initialize the interrupts
    init_can_interrupts();
}

// Priority must be a 2 bit number defining how high the priority of
// this message is vs the other ones queued to be sent. 0 is lowest
// priority, 3 is highest
void can_send(const can_msg_t* message) {
    // put the SID in buffer 0 of the DMA buffers
    can_msg_buf[0][0] = (message->sid << 2);

    // no extended identifier
    can_msg_buf[0][1] = 0;

    // set the length, other bits are zero because no EID
    can_msg_buf[0][2] = message->data_len;

    // copy data over. Note that this is a 16 bit part, so we
    // interlace the bytes of data. To see the structure that the CAN
    // module wants a CAN message to be in, look at datasheet section
    // 21.5
    can_msg_buf[0][3] = ((message->data[1] << 8) | message->data[0]);
    can_msg_buf[0][4] = ((message->data[3] << 8) | message->data[2]);
    can_msg_buf[0][5] = ((message->data[5] << 8) | message->data[4]);
    can_msg_buf[0][6] = ((message->data[7] << 8) | message->data[6]);

    // the message priority is always 3 (the highest)
    C1TR01CONbits.TX0PRI = 3;

    C1TR01CONbits.TXREQ0 = 1;
}

// Returns true if a message is ready to be sent
bool can_send_rdy(void) {
    return C1TR01CONbits.TXREQ0 == 0;
}

// Interrupt that runs whenever IFS2::C1IF is set
void __attribute__((__interrupt__, no_auto_psv)) _C1Interrupt(void) {
    // a transmit successfully finished, we abort the message so it
    // isn't set repeatedly
    if(C1INTFbits.TBIF) {
        C1INTFbits.TBIF = 0;
        C1TR01CONbits.TXABT0 = 1;
        return;
    }
    if (C1INTFbits.RBIF) {
        // check all receive buffers, trigger the callback with every
        // filled buffer
        can_msg_t cb_message;
        if(C1RXFUL1bits.RXFUL1) {
            buffer_to_msg(can_msg_buf[1], &cb_message);
            C1RXFUL1bits.RXFUL1 = 0;
            can_rcv_cb(&cb_message);
        }
        if(C1RXFUL1bits.RXFUL2) {
            buffer_to_msg(can_msg_buf[2], &cb_message);
            C1RXFUL1bits.RXFUL2 = 0;
            can_rcv_cb(&cb_message);
        }
        if(C1RXFUL1bits.RXFUL3) {
            buffer_to_msg(can_msg_buf[3], &cb_message);
            C1RXFUL1bits.RXFUL3 = 0;
            can_rcv_cb(&cb_message);
        }
        if(C1RXFUL1bits.RXFUL4) {
            buffer_to_msg(can_msg_buf[4], &cb_message);
            C1RXFUL1bits.RXFUL4 = 0;
            can_rcv_cb(&cb_message);
        }
        C1INTFbits.RBIF = 0;
        return;
    }
    if (C1INTFbits.RBOVIF) {
        // we weren't fast enough to catch all the messages
        // in the future, raise a warning now
        C1INTFbits.RBOVIF = 0;
        return;
    }
    if (C1INTFbits.ERRIF) {
        C1INTFbits.ERRIF = 0;
    }

    // there are no other CAN interrupts that we want to deal
    // with, so lower the main interrupt flag
    IFS2bits.C1IF = 0;
    return;
}

// Converts the DsPIC's internal CAN message buffer format into our
// message struct format
static void buffer_to_msg(const unsigned int *buffer, can_msg_t *received) {
    received->sid = ((buffer[0] >> 2) & 0x7ff);
    received->data_len = (buffer[2] & 0x0f);
    // fallthroughs are intentional
    switch(received->data_len) {
        case 8:
            received->data[7] = ((buffer[6] >> 8) & 0xff);
        case 7:
            received->data[6] =  (buffer[6] & 0xff);
        case 6:
            received->data[5] = ((buffer[5] >> 8) & 0xff);
        case 5:
            received->data[4] =  (buffer[5] & 0xff);
        case 4:
            received->data[3] = ((buffer[4] >> 8) & 0xff);
        case 3:
            received->data[2] =  (buffer[4] & 0xff);
        case 2:
            received->data[1] = ((buffer[3] >> 8) & 0xff);
        case 1:
            received->data[0] =  (buffer[3] & 0xff);
        case 0: default:
            return;
    }
}

static void init_dma_channels() {
    // use DMA channel 0 as the CAN Tx channel
    DMA0CONbits.CHEN = 0; // disable for setup
    DMA0CONbits.SIZE = 0; // set to word size, not byte size
    DMA0CONbits.DIR = 1; // read from RAM, write to CAN
    DMA0CONbits.AMODE = 0b10; // peripheral indirect addressing mode
    DMA0CONbits.MODE = 0; // continuous mode, disable ping-pong mode
    DMA0REQbits.IRQSEL = 0b01000110; // ECAN1-TX Data request
    DMA0CNT = 7; //8 words in a CAN message
    DMA0PAD = (volatile uint16_t) &C1TXD;
    DMA0STAL = (uint16_t) &can_msg_buf;
    DMA0STAH = (uint16_t) &can_msg_buf;
    DMA0CONbits.CHEN = 1; // we are go for DMA

    // enable DMA channel 1 as the CAN Rx channel
    DMA1CONbits.CHEN = 0;
    DMA1CONbits.SIZE = 0x0;
    DMA1CONbits.DIR = 0x0;
    DMA1CONbits.AMODE = 0x2;
    DMA1CONbits.MODE = 0x0;
    DMA1REQ = 34;
    DMA1CNT = 7;
    DMA1PAD = (volatile unsigned int)&C1RXD;
    DMA1STAL = (unsigned int) &can_msg_buf;
    DMA1STAH = (unsigned int) &can_msg_buf;
    DMA1CONbits.CHEN = 0x1;
}

static void init_can_interrupts() {
    __builtin_enable_interrupts();

    // put down a bunch of flags
    C1RXFUL1 = 0;
    C1RXFUL2 = 0;
    C1RXOVF1 = 0;
    C1RXOVF2 = 0;
    C1INTFbits.RBIF = 0;
    C1INTFbits.ERRIF = 0;
    C1INTFbits.TBIF = 0;

    // enable interrupts on transmit and receive. Disable error interrupts
    C1INTEbits.TBIE = 1;
    C1INTEbits.RBIE = 1;
    C1INTEbits.ERRIE = 0;

    // enable CAN1 interrupts
    IEC2bits.C1IE = 1;
}


// macro for the reduction of typing, only used in
// init_filters_and_masks. Sets filter n to point at mask 0, with
// filter SID 0, and cause the CAN buffer pointer for that filter to
// be n+1
#define INIT_FILTER(n) do {                          \
        /*point all filters at mask 0*/              \
        C1FMSKSEL1bits.F##n##MSK = 0;        \
        /*set the filter SID to 0*/                  \
        C1RXF##n##SID = 0;                  \
        /*point filter number at buffer (number+1)*/ \
        C1BUFPNT1bits.F##n##BP = ((n)+1); \
        } while(0)



// This driver does not currently support hardware filtering of
// incoming messages. All messages will be received, all will be
// handled the same through the callback.
static void init_filters_and_masks(void) {
    // set mask 0 to accept all messages
    C1RXM0SID = 0;

    // use 4 receive buffers in order to try to prevent overflows
    INIT_FILTER(0);
    INIT_FILTER(1);
    INIT_FILTER(2);
    INIT_FILTER(3);
}
