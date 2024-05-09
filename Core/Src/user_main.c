#include "main.h"
#include "user_main.h"
#include "buzz.h"
#include "nec.h"
#include "codec.h"

#define USE_BUZZ

#ifndef GATE_PASSWORD
#pragma message ( "no GATE_PASSWORD specified. Using 1234" )
#define GATE_PASSWORD "1234"
#else
#pragma message ( "GATE_PASSWORD specified." )
#endif

static const char password[4] = GATE_PASSWORD;

NEC_handler_t ir_handler = {
    .valid = 0,
    .state = IR_IDLE,
    .bit_count = -1,
    .result = 0
};

char codec_lookup(const uint32_t code);
void show_input_fail();
void show_input_success();
void show_pass_correct();
void show_pass_wrong();
bool check_input(const char* input, const char* password);

void user_main() {
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);

    uint8_t input_cursor = 0;
    char input[4] = {0};
    uint32_t last_input_time = HAL_GetTick();

    while(1) {
        if(ir_handler.new && ir_handler.valid) {
            const char signal = codec_lookup(ir_handler.result);
            ir_handler.new = false;
            last_input_time = HAL_GetTick();


            //handle input signal, and update the input register
            if(signal<='9' && signal>='0') {
                show_input_success();
                input[input_cursor] = signal;
                ++input_cursor;
            } else if (signal == '#') {
                show_input_success();
                input_cursor = 0;
            } else {
                show_input_fail();
            }

            //check if four inputs have been made
            if(input_cursor >= 4) {
                input_cursor = 0;
                if(check_input(input, password)) {
                    show_pass_correct();
                } else {
                    show_pass_wrong();
                }
            }
        } else if(input_cursor != 0 && (HAL_GetTick() - last_input_time > 5000 || last_input_time > HAL_GetTick())) {
            show_pass_wrong();
            input_cursor = 0;
        }
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

    const uint32_t capture_time_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    const uint32_t capture_time_2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

    switch(htim->Channel) {
    case HAL_TIM_ACTIVE_CHANNEL_1: //falling edge
        NEC_append_low(&ir_handler, capture_time_1 - capture_time_2);
        break;

    case HAL_TIM_ACTIVE_CHANNEL_2: //rising edge
        NEC_append_high(&ir_handler, capture_time_2);
        break;

    default:
        Error_Handler(); // not suppose to be here
        break;
    }
}

// might want to check out the potential bug
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    NEC_reset(&ir_handler);
}

void show_input_success() {
    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_SET);
#ifdef USE_BUZZ
    HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_1);
#endif
    HAL_Delay(100);
#ifdef USE_BUZZ
    HAL_TIM_OC_Stop(&htim3, TIM_CHANNEL_1);
#endif
    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_RESET);
}

void show_input_fail() {
    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_SET);
#ifdef USE_BUZZ
    HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_1);
#endif
    HAL_Delay(500);
#ifdef USE_BUZZ
    HAL_TIM_OC_Stop(&htim3, TIM_CHANNEL_1);
#endif
    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_RESET);
}

void show_pass_correct() {
    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_SET);
#ifdef USE_BUZZ
    HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
#endif
    HAL_Delay(300);
    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
#ifdef USE_BUZZ
    HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
#endif
}

void show_pass_wrong() {
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
#ifdef USE_BUZZ
    __HAL_TIM_SET_PRESCALER(&htim3, 2000-1);
    HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_1);
#endif
    HAL_Delay(1000);
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
#ifdef USE_BUZZ
    HAL_TIM_OC_Stop(&htim3, TIM_CHANNEL_1);
    __HAL_TIM_SET_PRESCALER(&htim3, 1000-1);
#endif
}

bool check_input(const char* input, const char* password) {
    for(int i = 0; i < 4; i++) {
        if(input[i] != password[i]) return false;
    }
    return true;
}

char codec_lookup(const uint32_t code) {
    switch(code) {
    case CODEC_0:
        return '0';
    case CODEC_1:
        return '1';
    case CODEC_2:
        return '2';
    case CODEC_3:
        return '3';
    case CODEC_4:
        return '4';
    case CODEC_5:
        return '5';
    case CODEC_6:
        return '6';
    case CODEC_7:
        return '7';
    case CODEC_8:
        return '8';
    case CODEC_9:
        return '9';
    case CODEC_STAR:
        return '*';
    case CODEC_HASH:
        return '#';
    case CODEC_UP:
        return 'U';
    case CODEC_DOWN:
        return 'D';
    case CODEC_LEFT:
        return 'L';
    case CODEC_RIGHT:
        return 'R';
    case CODEC_OK:
        return 'O';
    default:
        return '_';
    }
}