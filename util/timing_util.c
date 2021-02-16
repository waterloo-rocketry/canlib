#include "timing_util.h"

#if (CANLIB_BIT_TIME_US != 24)
#warning "the bit time that can.h is expecting is not what timing_util is expecting"
#endif

bool can_generate_timing_params(uint32_t can_frequency, can_timing_t *timing)
{
    // this function is designed to create a bit time of 24 microseconds
    switch (can_frequency) {
        case 48000000:
            timing->brp      = 47;
            timing->sjw      =  3;
            timing->btlmode  =  1;
            timing->sam      =  0;
            timing->seg1ph   =  4;
            timing->prseg    =  0;
            timing->seg2ph   =  4;
            return true;
        case 32000000:
            timing->brp      = 31;
            timing->sjw      =  3;
            timing->btlmode  =  1;
            timing->sam      =  0;
            timing->seg1ph   =  4;
            timing->prseg    =  0;
            timing->seg2ph   =  4;
            return true;
        case 12000000:
            timing->brp      = 11;
            timing->sjw      =  3;
            timing->btlmode  =  1;
            timing->sam      =  0;
            timing->seg1ph   =  4;
            timing->prseg    =  0;
            timing->seg2ph   =  4;
            return true;
        case 1000000:
            timing->brp      =  0;
            timing->sjw      =  3;
            timing->btlmode  =  1;
            timing->sam      =  0;
            timing->seg1ph   =  4;
            timing->prseg    =  0;
            timing->seg2ph   =  4;
            return true;
        default:
            // unhandled can frequency, just abort
            return false;
    }
}
