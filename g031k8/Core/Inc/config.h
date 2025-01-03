#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "stm32g0xx_hal.h"

#define T_NRST_Pin GPIO_PIN_2
#define T_NRST_GPIO_Port GPIOF
#define T_VCP_TX_Pin GPIO_PIN_2
#define T_VCP_TX_GPIO_Port GPIOA
#define T_VCP_RX_Pin GPIO_PIN_3
#define T_VCP_RX_GPIO_Port GPIOA
#define LD3_Pin GPIO_PIN_6
#define LD3_GPIO_Port GPIOC
#define T_JTMS_Pin GPIO_PIN_13
#define T_JTMS_GPIO_Port GPIOA
#define T_JTCK_Pin GPIO_PIN_14
#define T_JTCK_GPIO_Port GPIOA

#define ADC_CH0_Pin GPIO_PIN_0
#define ADC_CH0_GPIO_Port GPIOA

#define ADC_CH1_Pin GPIO_PIN_1
#define ADC_CH1_GPIO_Port GPIOA

#define ADC_CH4_Pin GPIO_PIN_4
#define ADC_CH4_GPIO_Port GPIOA

#define ADC_CH5_Pin GPIO_PIN_5
#define ADC_CH5_GPIO_Port GPIOA

#define TIM14_CH1_Pin GPIO_PIN_7
#define TIM14_CH1_GPIO_Port GPIOA

#define ISR_MEAS_Pin GPIO_PIN_12
#define ISR_MEAS_GPIO_Port GPIOA

#define SYM_PROC_Pin GPIO_PIN_11
#define SYM_PROC_GPIO_Port GPIOA

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;
extern UART_HandleTypeDef huart2;

void System_Init(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_ADC1_Init(void);
void MX_TIM14_Init(void);
void MX_TIM16_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void Error_Handler(void);


#endif /* INC_CONFIG_H_ */
