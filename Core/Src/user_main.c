#include "main.h"
#include "user_main.h"
#include "buzz.h"
#include "stdbool.h"
#include "nec.h"
#include "codec.h"

NEC_handler_t ir_handler = {
    .valid = 0,
    .state = IR_IDLE,
    .bit_count = -1,
    .result = 0
};


uint32_t test[256] = {0};
uint8_t count = 0;
HAL_TIM_ActiveChannel hl[256] = {0};
uint8_t state[256] = {0};

void user_main() {
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
    while(1) {
        if(ir_handler.new && ir_handler.valid) {
            ir_handler.new = false;
            switch(ir_handler.result) {
                case CODEC_RIGHT:
                    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_SET);
                    HAL_Delay(100);
                    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_RESET);
                    break;
                case CODEC_OK:
                    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
                    HAL_Delay(100);
                    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
                    break;
                case CODEC_UP:
                    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_SET);
                    HAL_Delay(100);
                    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
                    break;
                case CODEC_DOWN:
                    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_SET);
                    HAL_Delay(100);
                    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
                    break;
                case CODEC_HASH:
                    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_SET);
                    HAL_Delay(100);
                    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_RESET);
                    break;
                case CODEC_LEFT:
                    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_SET);
                    HAL_Delay(100);
                    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_RESET);
                    break;
                default:
                    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_SET);
                    HAL_Delay(100);
                    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_RESET);
                    break;
            }
            
        }
    }
}


void door_open() {
    HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

    const uint32_t capture_time_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    const uint32_t capture_time_2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
    hl[count] = htim->Channel;
    state[count] = ir_handler.state;


    switch(htim->Channel) {
    case HAL_TIM_ACTIVE_CHANNEL_1: //falling edge
        test[count] = capture_time_1;
        count++;
        NEC_append_low(&ir_handler, capture_time_1 - capture_time_2);
        break;

    case HAL_TIM_ACTIVE_CHANNEL_2: //rising edge
        test[count] = capture_time_2;
        count++;
        NEC_append_high(&ir_handler, capture_time_2);
        break;

    default:
        Error_Handler(); // not suppose to be here
        break;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    NEC_reset(&ir_handler);
}