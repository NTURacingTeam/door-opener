#include "stm32f1xx_hal.h"
#include "play_note.h"
#include "music_note.h"

#define bpm 210

void regular_note(TIM_HandleTypeDef* htim, uint32_t TIM_Channel, const note_t* note) {
    const uint32_t note_length = 1000/(bpm/60) *note->length_num /note->length_den;

    //start the note, or stop it depending on the note type
    if(note->type != REST) {
        __HAL_TIM_SET_PRESCALER(htim, note->period);
        HAL_TIM_OC_Start(htim, TIM_Channel);
    } else {
        HAL_TIM_OC_Stop(htim, TIM_Channel);
    }

    //wait for a while then stop the note. Wait time depends on the note type
    switch(note->type) {
    case DISCONTINUOUS:
        HAL_Delay(note_length * 4/5);
        HAL_TIM_OC_Stop(htim, TIM_Channel);
        HAL_Delay(note_length /5);
        break;
    case STACCATO:
        HAL_Delay(note_length /5);
        HAL_TIM_OC_Stop(htim, TIM_Channel);
        HAL_Delay(note_length * 4/5);
        break;
    case CONTINUOUS:
    case REST:
    default:
        HAL_Delay(note_length);
        break;
    }
    
}

const note_t song[] = {
    {
        .length_den = 1,
        .length_num = 1,
        .type = CONTINUOUS,
        .period = 1000000/NOTE_A
    },
    {
        .length_den = 1,
        .length_num = 1,
        .type = CONTINUOUS,
        .period = 1000000/NOTE_F
    },
    {
        .length_den = 1,
        .length_num = 1,
        .type = CONTINUOUS,
        .period = 1000000/NOTE_C
    },
    {
        .length_den = 1,
        .length_num = 1,
        .type = CONTINUOUS,
        .period = 1000000/NOTE_F
    },
    {
        .length_den = 1,
        .length_num = 1,
        .type = CONTINUOUS,
        .period = 1000000/NOTE_G
    },
    {
        .length_den = 1,
        .length_num = 2,
        .type = CONTINUOUS,
        .period = 1000000/NOTE_CC
    },
    {
        .length_den = 1,
        .length_num = 1,
        .type = CONTINUOUS,
        .period = 1000000/NOTE_G
    },
    {
        .length_den = 1,
        .length_num = 1,
        .type = CONTINUOUS,
        .period = 1000000/NOTE_A
    },
    {
        .length_den = 1,
        .length_num = 1,
        .type = CONTINUOUS,
        .period = 1000000/NOTE_G
    },
    {
        .length_den = 1,
        .length_num = 1,
        .type = CONTINUOUS,
        .period = 1000000/NOTE_C
    },
    {
        .length_den = 1,
        .length_num = 2,
        .type = CONTINUOUS,
        .period = 1000000/NOTE_F
    },
};

void play_song(TIM_HandleTypeDef* htim, uint32_t TIM_Channel) {
    for(int8_t i = 0; i < sizeof(song)/sizeof(note_t); ++i) {
        regular_note(htim, TIM_Channel, &(song[i]));
    }
    HAL_TIM_OC_Stop(htim, TIM_Channel);
}