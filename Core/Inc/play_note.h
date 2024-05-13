#ifndef __PLAY_NOTE_H
#define __PLAY_NOTE_H

#include "stdint.h"
#include "stm32f1xx_hal.h"

typedef struct {
    enum {
        CONTINUOUS,
        DISCONTINUOUS,
        STACCATO,
        REST
    } type;
    uint8_t length_num;
    uint8_t length_den;
    uint32_t period;
} note_t;

void play_song(TIM_HandleTypeDef* htim, uint32_t TIM_Channel);

#endif