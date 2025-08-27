#ifndef INC_CUSTOM_CALLBACKS_H_
#define INC_CUSTOM_CALLBACKS_H_

#include <stdint.h>

#include "state_machine.h"
#include "checking_functions.h"
#include "CC_and_PC.h"
#include <oscillator.h>
#include "input_capture.h"
#include "MIDI_defines.h"
#include "UART.h"
#include "debug.h"
#include "MIDI.h"
#include "main.h"
#include "LED.h"

void ADC_DMA_conversion_complete_callback(ADC_HandleTypeDef *hadc);
void TIM16_callback(TIM_HandleTypeDef *htim);
void TIM2_ch1_IP_capture_callback(TIM_HandleTypeDef *htim);
void TIM2_ch1_overflow_callback(TIM_HandleTypeDef *htim);
void TIM3_ch1_IP_capture_measurement_reelapse_callback(TIM_HandleTypeDef *htim);
void __attribute__((optimize("O0")))LPTIM1_callback(LPTIM_HandleTypeDef *hlptim);
void TIM17_callback(TIM_HandleTypeDef *htim);
void __attribute__((optimize("O0")))TIM14_callback(TIM_HandleTypeDef *htim);
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin);
void UART2_TX_transfer_complete_callback(UART_HandleTypeDef *huart);
void __attribute__((optimize("O0")))UART2_RX_transfer_complete_callback(UART_HandleTypeDef *huart);
void DMA_M2M_Callback(DMA_HandleTypeDef *hdma);

#endif /* INC_CUSTOM_CALLBACKS_H_ */
