
#include "main.h"


int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();

  while (1)
  {
	  HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

	  HAL_Delay(5000);

  }
}
