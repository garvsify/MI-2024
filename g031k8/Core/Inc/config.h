#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "stm32g0xx_hal.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart2;

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_USART2_UART_Init(void);
void MX_ADC1_Init(void);
void MX_TIM1_Init(void);



#endif /* INC_CONFIG_H_ */
