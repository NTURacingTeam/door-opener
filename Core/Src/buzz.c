#include "buzz.h"
#include "stm32f1xx_hal.h"

void buzz_on(TIM_HandleTypeDef* htim, uint32_t channel) {
    HAL_TIM_OC_Start(htim, channel);
}

void buzz_off(TIM_HandleTypeDef* htim, uint32_t channel) {
    HAL_TIM_OC_Start(htim, channel);
}