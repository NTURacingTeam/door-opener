#include "stm32f1xx_hal.h"


#define bpm 60

typedef enum {
    CONTINUOUS,
    DISCONTINUOUS,
    STACCATO,
    REST
} note_type_t;

void regular_note(TIM_HandleTypeDef* htim, uint32_t TIM_Channel, uint8_t length_num, uint8_t length_den, note_type_t type, uint32_t period) {
    const uint32_t note_length = 1000/(bpm/60) *length_num /length_den;

    //start the note, or stop it depending on the note type
    if(type != REST) {
        __HAL_TIM_SET_PRESCALER(htim, period);
        HAL_TIM_OC_Start(htim, TIM_Channel);
    } else {
        HAL_TIM_OC_Stop(htim, TIM_Channel);
    }

    //wait for a while then stop the note. Wait time depends on the note type
    switch(type) {
    DISCONTINUOUS:
        HAL_Delay(note_length * 4/5);
        HAL_TIM_OC_Stop(htim, TIM_Channel);
        HAL_Delay(note_length /5);
        break;
    STACCATO:
        HAL_Delay(note_length /5);
        HAL_TIM_OC_Stop(htim, TIM_Channel);
        HAL_Delay(note_length * 4/5);
        break;
    CONTINUOUS:
    REST:
    default:
        HAL_Delay(note_length);
        break;
    }
    
}