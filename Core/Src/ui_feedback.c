#include "ui_feedback.h"
#include "music_note.h"
#include "stm32f1xx_hal.h"
#include "play_note.h"


void open_door(GPIO_TypeDef* door_port, uint16_t door_pin) {
#ifdef USE_BUZZ
    HAL_GPIO_WritePin(door_port, door_pin, GPIO_PIN_SET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(door_port, door_pin, GPIO_PIN_RESET);
#endif
}

void show_input_success(GPIO_TypeDef* light_port, uint16_t light_pin, TIM_HandleTypeDef *htim, uint32_t TIM_Channel) {
    HAL_GPIO_WritePin(light_port, light_pin, GPIO_PIN_SET);
#ifdef USE_BUZZ
    __HAL_TIM_SET_PRESCALER(htim, 1000000/NOTE_CC);
    HAL_TIM_OC_Start(htim, TIM_Channel);
#endif
    HAL_Delay(100);
#ifdef USE_BUZZ
    HAL_TIM_OC_Stop(htim, TIM_Channel);
#endif
    HAL_GPIO_WritePin(light_port, light_pin, GPIO_PIN_RESET);
}

void show_input_fail(GPIO_TypeDef* light_port, uint16_t light_pin, TIM_HandleTypeDef *htim, uint32_t TIM_Channel) {
    HAL_GPIO_WritePin(light_port, light_pin, GPIO_PIN_SET);
#ifdef USE_BUZZ
    __HAL_TIM_SET_PRESCALER(htim, 1000000/NOTE_CC);
    HAL_TIM_OC_Start(htim, TIM_Channel);
#endif
    HAL_Delay(500);
#ifdef USE_BUZZ
    HAL_TIM_OC_Stop(htim, TIM_Channel);
#endif
    HAL_GPIO_WritePin(light_port, light_pin, GPIO_PIN_RESET);
}

void show_pass_correct(GPIO_TypeDef* light_port, uint16_t light_pin, TIM_HandleTypeDef *htim, uint32_t TIM_Channel) {
    HAL_GPIO_WritePin(light_port, light_pin, GPIO_PIN_SET);
    HAL_Delay(300);
#ifdef USE_BUZZ
    play_song(htim, TIM_Channel);
#endif
    HAL_GPIO_WritePin(light_port, light_pin, GPIO_PIN_RESET);
}


void show_pass_wrong(GPIO_TypeDef* light_port, uint16_t light_pin, TIM_HandleTypeDef *htim, uint32_t TIM_Channel) {
    HAL_GPIO_WritePin(light_port, light_pin, GPIO_PIN_SET);
#ifdef USE_BUZZ
    __HAL_TIM_SET_PRESCALER(htim, 1000000/NOTE_E);
    HAL_TIM_OC_Start(htim, TIM_Channel);
#endif
    HAL_Delay(1000);
    HAL_GPIO_WritePin(light_port, light_pin, GPIO_PIN_RESET);
#ifdef USE_BUZZ
    HAL_TIM_OC_Stop(htim, TIM_Channel);
#endif
}
