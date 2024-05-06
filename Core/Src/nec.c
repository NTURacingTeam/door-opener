#include "stdbool.h"
#include "stdint.h"
#include "nec.h"

void NEC_reset(NEC_handler_t* handler) {
    handler->state = IR_IDLE;
    handler->bit_count = -1;
    handler->result = 0UL;
    handler->valid = 0;
    handler->new = 0;
}

static inline bool inTolerance(const uint32_t input, const uint32_t target, const uint32_t tolerance) {
    return input < target + tolerance && input > target - tolerance;
}

void NEC_append_low(NEC_handler_t* handler, const uint32_t time) {
    switch(handler->state) {

    case IR_IDLE:
        handler->state = IR_START;
        break;

    case IR_START:
        if(inTolerance(time, NEC_TIME_START_LOW, NEC_TIME_TOL)) {
            handler->bit_count = -1;
            handler->valid = 0;
            handler->state = IR_RECEIVING;
        } else NEC_reset(handler);
        break;

    case IR_RECEIVING:
        if(inTolerance(time, NEC_TIME_0_LOW, NEC_TIME_TOL)) {
            handler->bit_count++;
            handler->result &= ~(1UL << handler->bit_count);
            if(handler->bit_count == 31) {
                handler->state = IR_IDLE;
                handler->valid = 1;
                handler->new = 1;
            }
        } else if (inTolerance(time, NEC_TIME_1_LOW, NEC_TIME_TOL)) {
            handler->bit_count++;
            handler->result |= (1UL << handler->bit_count);
            if(handler->bit_count == 31) {
                handler->state = IR_IDLE;
                handler->valid = 1;
                handler->new = 1;
            }
        } else NEC_reset(handler);
    }
}

void NEC_append_high(NEC_handler_t* handler, const uint32_t time) {
    switch(handler->state) {
    case IR_IDLE:
        break; // not suppose to happen though

    case IR_START: 
        if(inTolerance(time, NEC_TIME_START_HIGH, NEC_TIME_TOL)) {
            ;
        } else NEC_reset(handler);
        break;
    
    case IR_RECEIVING:
        if(inTolerance(time, NEC_TIME_HIGH, NEC_TIME_TOL)) {
            ;
        } else NEC_reset(handler);
        break;
    }
}