#ifndef __UI_FEEDBACK_H
#define __UI_FEEDBACK_H
#include "stm32f1xx_hal.h"

void show_input_success (GPIO_TypeDef* light_port, uint16_t light_pin, TIM_HandleTypeDef *htim, uint32_t TIM_Channel);
void show_input_fail    (GPIO_TypeDef* light_port, uint16_t light_pin, TIM_HandleTypeDef *htim, uint32_t TIM_Channel);
void show_pass_correct  (GPIO_TypeDef* light_port, uint16_t light_pin, TIM_HandleTypeDef *htim, uint32_t TIM_Channel);
void show_pass_wrong    (GPIO_TypeDef* light_port, uint16_t light_pin, TIM_HandleTypeDef *htim, uint32_t TIM_Channel);

#endif