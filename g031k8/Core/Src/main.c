//INCLUDES
#include "system.h"

//VARIABLES
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim14;
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

UART_HandleTypeDef huart2;

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM14_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();

  //Set callback function for TIM16 (freq. gen.) to the callback function in TIMx_callback.c for TIM16.
  //I believe the correct CallbackID is HAL_TIM_OC_DELAY_ELAPSED_CB_ID, but if this doesn't work maybe
  //HAL_TIM_PERIOD_ELAPSED_CB_ID will work. This should be basically the same because we've set up TIM16
  //in Output Compare mode, where the ARR and CRR are the same.
  HAL_TIM_RegisterCallback(&htim16, HAL_TIM_OC_DELAY_ELAPSED_CB_ID, &TIM16_callback);

  //Set callback function for TIM16 (adc trig.) to the callback function in TIMx_callback.c for TIM17.
  //I believe the correct CallbackID is HAL_TIM_OC_DELAY_ELAPSED_CB_ID, but if this doesn't work maybe
  //HAL_TIM_PERIOD_ELAPSED_CB_ID will work. This should be basically the same because we've set up TIM16
  //in Output Compare mode, where the ARR and CRR are the same.
  HAL_TIM_RegisterCallback(&htim17, HAL_TIM_OC_DELAY_ELAPSED_CB_ID, &TIM17_callback);


  while (1)
  {

  }
}

