#include "timing_util.h"

#if (CANLIB_BIT_TIME_US != 24)
#warning "the bit time that can.h is expecting is not what timing_util is expecting"
#endif

bool can_generate_timing_params(uint32_t can_frequency, can_timing_t *timing)
{
    // this function is designed to create a bit time of 24 microseconds
    // page 592 onwards of the PIC18F26K83 datasheet describes bit timming
    // tageting a nominal bit time of 8 Tq
    // sync(1Tq) + prop_seg + phase_seg_1 + phase_seg_2
    // Tq = 2*(brp + 1)/fosc
    switch (can_frequency) {
        case 64000000:
            timing->brp      =  3;
            timing->sjw      =  3;
            timing->btlmode  =  1;
            timing->sam      =  0;
            timing->seg1ph   =  2; //phase_seg_1 = 3Tq
            timing->prseg    =  0; //prop_seg = 1Tq
            timing->seg2ph   =  2; //phase_seg_2 = 3Tq
            return true;
        case 48000000:
            timing->brp      = 2;
            timing->sjw      =  3;
            timing->btlmode  =  1;
            timing->sam      =  0;
            timing->seg1ph   =  2;
            timing->prseg    =  0;
            timing->seg2ph   =  2;
            return true;
        case 32000000:
            timing->brp      = 1;
            timing->sjw      =  3;
            timing->btlmode  =  1;
            timing->sam      =  0;
            timing->seg1ph   =  2;
            timing->prseg    =  0;
            timing->seg2ph   =  2;
            return true;
        case 16000000:
            timing->brp      =  0;
            timing->sjw      =  3;
            timing->btlmode  =  1;
            timing->sam      =  0;
            timing->seg1ph   =  2;
            timing->prseg    =  0;
            timing->seg2ph   =  2;
            return true;
/*        case 1000000:
            timing->brp      =  0;
            timing->sjw      =  3;
            timing->btlmode  =  1;
            timing->sam      =  0;
            timing->seg1ph   =  4;
            timing->prseg    =  0;
            timing->seg2ph   =  4;
            return true;         */
        default:
            // unhandled can frequency, just abort
            return false;
    }
}
