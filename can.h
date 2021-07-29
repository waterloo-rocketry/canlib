/*
 * file that defines structures that are common across all 3 of
 * Waterloo Rocketry's CAN implementations
 */

#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>
#include <stdbool.h>

#define CANLIB_VERSION            "v0.6.0"
#define CANLIB_BIT_TIME_US        24

// Timing parameters
typedef struct {
    // BaudRate Prescaler
    uint8_t brp;
    // Synchronization Jump Width
    uint8_t sjw;

    // sample once or three times
    uint8_t sam;
    // phase segment 1 bits
    uint8_t seg1ph;
    // phase segment 2 bits
    uint8_t seg2ph;
    // propagation time segment bits
    uint8_t prseg;

    // Phase segment 2 time select bit. If true, then use seg2ph,
    // otherwise take minimum viable phase length
    bool btlmode;
} can_timing_t;

// Things that are in a CAN message
typedef struct {
    // Standard Identifier - 11 bits long
    uint16_t sid;
    // How many bytes are used in data
    uint8_t data_len;
    // the data you want to transmit
    uint8_t data[8];
} can_msg_t;

#endif // CAN_H_
