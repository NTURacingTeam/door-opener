#ifndef __NEC_H
#define __NEC_H

// burst time for NEC IR codec, in microseconds
#define NEC_TIME_START_HIGH 9000
#define NEC_TIME_START_LOW 4500
#define NEC_TIME_HIGH 562
#define NEC_TIME_1_LOW 1688
#define NEC_TIME_0_LOW 563
#define NEC_TIME_TOL 100 //tolerance for burst time

typedef struct {
    enum {
        IR_IDLE,
        IR_START,
        IR_RECEIVING,
    } state;
    bool valid;
    bool new;
    int8_t bit_count;
    uint32_t result;

} NEC_handler_t;

void NEC_reset(NEC_handler_t*);

void NEC_append_low(NEC_handler_t* handler, const uint32_t time);
void NEC_append_high(NEC_handler_t* handler, const uint32_t time);

#endif