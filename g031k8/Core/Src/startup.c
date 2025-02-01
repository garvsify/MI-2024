#include "startup.h"

uint8_t Startup(void){

	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE); //make sure the overflow (update) interrupt is enabled for TIM2
	__HAL_TIM_ENABLE_IT(&htim16, TIM_IT_UPDATE); //make sure the overflow (update) interrupt is enabled for TIM16

	//SET DEFAULT PIN STATES
	HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1); //latch high the debounced o/p
	HAL_GPIO_WritePin(HACK_POT_HIGH_GPIO_Port, HACK_POT_HIGH_Pin, 1);
	HAL_GPIO_WritePin(HACK_POT_LOW_GPIO_Port, HACK_POT_LOW_Pin, 0);


	//GET ADC VALUES
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

	//WAIT
	while(initial_ADC_conversion_complete == NO){}; //wait while first ADC conversion is ongoing - raw and final values will be computed within this time

	// re-initialise all values in delay line storage array to middle value of wave (if sine/triangle mode) or bottom of wave if square mode, as they are initialised to 0 on startup
	for(uint16_t i = 0; i < FINAL_INDEX + 2; i++){ //513

		if(params.waveshape == SQUARE_MODE){

			delay_line.duty_delay_line_storage_array[i] = PWM_DUTY_VALUE_MAX - ((params.depth * PWM_DUTY_VALUE_MAX) >> DEPTH_ADC_RESOLUTION);
		}
		else{

			delay_line.duty_delay_line_storage_array[i] = PWM_DUTY_VALUE_MAX - (((params.depth * PWM_DUTY_VALUE_MAX) >> DEPTH_ADC_RESOLUTION) >> 1);
		}
	}

	//PREPARE OSCILLATORS
	Calculate_Next_Main_Oscillator_Values(&params, (enum Next_Values_Processing_Mode)STARTUP_MODE);
	Write_Next_Main_Oscillator_Values_to_Delay_Line(&params, &delay_line);
	Set_Oscillator_Values(&params);

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_Gen_Timer_Main_and_Secondary_Oscillators(&htim1, TIM_CHANNEL_2, TIM_CHANNEL_4);
	Start_Freq_Gen_Timer();

	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();

	//START UART RECEIVE
	Start_UART_Receive();

	//START IP CAP
	Start_Input_Capture_Timer();


	#if TAPCLK_ON_OR_OFF == ON

		//ENABLE TAP-TEMPO SWITCH CHECKING
		HAL_LPTIM_SetOnce_Start_IT(&hlptim1, LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK, LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK);


	#endif

	return 1;
}

