//INCLUDES
#include "system.h"

//CONSTANTS
const uint16_t sine_wavetable[512]={512,518,524,530,537,543,549,555,562,568,574,580,587,593,599,605,611,617,624,630,636,642,648,654,660,666,672,678,684,690,696,701,707,713,719,725,730,736,741,747,753,758,764,769,774,780,785,790,796,801,806,811,816,821,826,831,836,841,846,850,855,860,864,869,873,878,882,886,890,895,899,903,907,911,915,919,922,926,930,933,937,940,944,947,950,953,957,960,963,966,968,971,974,977,979,982,984,986,989,991,993,995,997,999,1001,1003,1004,1006,1008,1009,1011,1012,1013,1014,1015,1017,1017,1018,1019,1020,1021,1021,1022,1022,1022,1023,1023,1023,1023,1023,1023,1023,1022,1022,1022,1021,1021,1020,1019,1018,1017,1017,1015,1014,1013,1012,1011,1009,1008,1006,1004,1003,1001,999,997,995,993,991,989,986,984,982,979,977,974,971,968,966,963,960,957,953,950,947,944,940,937,933,930,926,922,919,915,911,907,903,899,895,890,886,882,878,873,869,864,860,855,850,846,841,836,831,826,821,816,811,806,801,796,790,785,780,774,769,764,758,753,747,741,736,730,725,719,713,707,701,696,690,684,678,672,666,660,654,648,642,636,630,624,617,611,605,599,593,587,580,574,568,562,555,549,543,537,530,524,518,512,505,499,493,486,480,474,468,461,455,449,443,436,430,424,418,412,406,399,393,387,381,375,369,363,357,351,345,339,333,327,322,316,310,304,298,293,287,282,276,270,265,259,254,249,243,238,233,227,222,217,212,207,202,197,192,187,182,177,173,168,163,159,154,150,145,141,137,133,128,124,120,116,112,108,104,101,97,93,90,86,83,79,76,73,70,66,63,60,57,55,52,49,46,44,41,39,37,34,32,30,28,26,24,22,20,19,17,15,14,12,11,10,9,8,6,6,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,6,6,8,9,10,11,12,14,15,17,19,20,22,24,26,28,30,32,34,37,39,41,44,46,49,52,55,57,60,63,66,70,73,76,79,83,86,90,93,97,101,104,108,112,116,120,124,128,133,137,141,145,150,154,159,163,168,173,177,182,187,192,197,202,207,212,217,222,227,233,238,243,249,254,259,265,270,276,282,287,293,298,304,310,316,322,327,333,339,345,351,357,363,369,375,381,387,393,399,406,412,418,424,430,436,443,449,455,461,468,474,480,486,493,499,505};
const uint16_t tri_wavetable[512]={512,516,520,524,528,532,536,540,544,548,552,556,560,564,568,572,576,580,584,588,592,596,600,604,608,612,616,620,624,628,632,636,640,644,648,652,656,660,664,668,672,676,680,684,688,692,696,700,704,708,712,716,720,724,728,732,736,740,744,748,752,756,760,764,768,771,775,779,783,787,791,795,799,803,807,811,815,819,823,827,831,835,839,843,847,851,855,859,863,867,871,875,879,883,887,891,895,899,903,907,911,915,919,923,927,931,935,939,943,947,951,955,959,963,967,971,975,979,983,987,991,995,999,1003,1007,1011,1015,1019,1023,1019,1015,1011,1007,1003,999,995,991,987,983,979,975,971,967,963,959,955,951,947,943,939,935,931,927,923,919,915,911,907,903,899,895,891,887,883,879,875,871,867,863,859,855,851,847,843,839,835,831,827,823,819,815,811,807,803,799,795,791,787,783,779,775,771,767,763,759,755,751,747,743,739,735,731,727,723,719,715,711,707,703,699,695,691,687,683,679,675,671,667,663,659,655,651,647,643,639,635,631,627,623,619,615,611,607,603,599,595,591,587,583,579,575,571,567,563,559,555,551,547,543,539,535,531,527,523,519,515,512,508,504,500,496,492,488,484,480,476,472,468,464,460,456,452,448,444,440,436,432,428,424,420,416,412,408,404,400,396,392,388,384,380,376,372,368,364,360,356,352,348,344,340,336,332,328,324,320,316,312,308,304,300,296,292,288,284,280,276,272,268,264,260,256,252,248,244,240,236,232,228,224,220,216,212,208,204,200,196,192,188,184,180,176,172,168,164,160,156,152,148,144,140,136,132,128,124,120,116,112,108,104,100,96,92,88,84,80,76,72,68,64,60,56,52,48,44,40,36,32,28,24,20,16,12,8,4,0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,236,240,244,248,252,256,260,264,268,272,276,280,284,288,292,296,300,304,308,312,316,320,324,328,332,336,340,344,348,352,356,360,364,368,372,376,380,384,388,392,396,400,404,408,412,416,420,424,428,432,436,440,444,448,452,456,460,464,468,472,476,480,484,488,492,496,500,504,508};
const uint16_t TIM16_prescalers[6] = {2048, 1024, 512, 256, 128, 64}; //2048 is used only by base prescaler index == 1 during symmetry adjustment
const uint8_t num_ADC_conversions = sizeof(ADCResultsDMA) / sizeof(ADCResultsDMA[0]);

//VARIABLE DEFINITIONS
volatile uint16_t ADCResultsDMA[5] = {0};
volatile enum Validate initial_ADC_conversion_complete = NO;
volatile enum Input_Capture_Event input_capture_event = FIRST;
volatile uint32_t TIM2_ch1_input_capture_value;
volatile enum Validate input_capture_measurement_is_ongoing = NO;
volatile enum Validate input_capture_measurement_reelapse_is_ongoing = NO;
volatile uint16_t interrupt_period = 0;
volatile uint8_t speed_pot_adc_measurement_num = 0;
volatile enum Validate is_very_first_oscillation = YES;
volatile enum Validate UART_DMA_TX_is_complete = YES;
uint8_t tap_tempo_switch_state_counter = TAP_TEMPO_SWITCH_CONFIDENCE_COUNT;
enum Tap_Tempo_Switch_State tap_tempo_switch_state = NOT_DEPRESSED;
volatile enum Validate input_capture_processing_can_be_started = NO;
volatile enum Validate external_clock_mode_is_active = NO;
volatile enum Validate tap_tempo_mode_is_active = NO;

//STRUCT VARIABLES
struct Params params = {0};
struct Params params_to_be_loaded = {0};
struct Delay_Line delay_line = {.duty_delay_line_storage_array = 0, //one index larger than the number of indexes (wave samples) to allow us to 'wrap' the array into a kind of circular buffer - this is reinitialised to mid-scale on runtime
								.duty_delay_line_start_offset = 1,  //initial value is 1st index - to give us space to fill index 0
								.duty_delay_line_finish_offset = FINAL_INDEX + 1, //initial value is 512th index, one larger than the total number of samples in 1 oscillation
								.duty_delay_line_read_pointer_offset = 0}; //determined in running

//FUNCTION DEFINITIONS
uint8_t Global_Interrupt_Enable(void){

	__enable_irq();
	return 1;
}

uint8_t Global_Interrupt_Disable(void){

	__disable_irq();
	return 1;
}

uint8_t Startup(void){

	// re-initialise all values in delay line storage array to 512 (mid scale) as they are initialised to 0 on startup
	for(uint16_t i = 0; i < FINAL_INDEX + 1; i++){

		delay_line.duty_delay_line_storage_array[i] = INITIAL_PWM_VALUE;
	}

	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE); //make sure the overflow (update) interrupt is enabled for TIM2

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

	//WAIT
	while(initial_ADC_conversion_complete == NO){}; //wait while first ADC conversion is ongoing

	HAL_ADC_Stop_DMA(&hadc1);

	//SET DEFAULT PIN STATES
	HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1); //latch high the debounced o/p
	HAL_GPIO_WritePin(HACK_POT_HIGH_GPIO_Port, HACK_POT_HIGH_Pin, 1);
	HAL_GPIO_WritePin(HACK_POT_LOW_GPIO_Port, HACK_POT_LOW_Pin, 0);

	return 1;
}

uint8_t Start_PWM_Gen_Timer_Main_and_Secondary_Oscillators(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel_1, uint32_t PWM_TIM_channel_2)
{
	uint8_t ok_AND = 1;
	ok_AND &= HAL_TIM_Base_Start(TIM);
	ok_AND &= HAL_TIM_PWM_Start(TIM, PWM_TIM_channel_1); //start PWM
	ok_AND &= HAL_TIM_PWM_Start(TIM, PWM_TIM_channel_2); //start PWM

	if(ok_AND != HAL_OK){

		Error_Handler();
	}

	return ok_AND;
}

uint8_t Start_Freq_Gen_Timer(void)
{
	uint8_t ok = Start_OC_TIM(&htim16, TIM_CHANNEL_1); //start freq. gen.

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Start_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t OC_TIM_channel){

	uint8_t ok = HAL_TIM_OC_Start_IT(TIM, OC_TIM_channel); //_IT variant of function
	//means the timer will generate an interrupt on delay_elapsed (CNT = CCR) condition

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Stop_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t OC_TIM_channel){

	uint8_t ok = HAL_TIM_OC_Stop_IT(TIM, OC_TIM_channel);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Start_IC_TIM(TIM_HandleTypeDef *TIM, uint32_t IC_TIM_channel){

	uint8_t ok_AND = HAL_TIM_Base_Start_IT(&htim2);
	ok_AND &= HAL_TIM_IC_Start_IT(&htim2, IC_TIM_channel);

	if(ok_AND != HAL_OK){

		Error_Handler();
	}

	return ok_AND;
}

uint8_t Start_Input_Capture_Timer(void){

	uint8_t ok = Start_IC_TIM(&htim2, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(struct Params* params_ptr){

	uint32_t speed_control = 0;
	uint8_t how_many_128 = 0;

    //speed_control = (speed_adc_10_bit/1024)*'range macro'
    speed_control = params_ptr->speed * NUMBER_OF_FREQUENCY_STEPS;
    speed_control = speed_control >> SYMMETRY_ADC_RESOLUTION;

    how_many_128 = (uint8_t)(speed_control >> 7); //divide by 128, i.e. return how many 128s go into the speed_control
    params_ptr->raw_start_value = (uint16_t)(speed_control - (uint16_t)(how_many_128 << 7)); //how_many_128*128
    params_ptr->raw_prescaler = SLOWEST_SPEED_PRESCALER >> how_many_128;

    return 1;
}

uint8_t Adjust_TIM16_Prescaler(struct Params* params_ptr){

    if(params_ptr->prescaler_adjust == MULTIPLY_BY_TWO){
    	params_ptr->final_prescaler = params_ptr->raw_prescaler << 1;
    }
    else if(params_ptr->prescaler_adjust == DO_NOTHING){
    	params_ptr->final_prescaler = params_ptr->raw_prescaler;
    }
    return 1;
}

uint8_t Process_TIM16_Final_Start_Value_and_Final_Prescaler(struct Params* params_ptr){

    #if SYMMETRY_ON_OR_OFF == ON

		enum TIM16_final_start_value_Oscillation_Mode TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
		enum Symmetry_Type symmetry_type_for_halfcycle = SHORTEN;

		uint8_t pot_rotation_corrected = 0;
		enum Symmetry_Status symmetry_status = CW;

		//DETERMINE IF CW OR CCW SYMMETRY POT ROTATION
		if(params_ptr->symmetry < SYMMETRY_ADC_HALF_SCALE){ //adc = 0-127
			symmetry_status = CW;
		}
		else{ //adc is 128-255
			symmetry_status = CCW;
		}

		//DETERMINE WHETHER TO SHORTEN OR LENGTHEN BASED ON CURRENT INDEX AND SYMMETRY POT POSITION
		if((params_ptr->waveshape == SINE_MODE) || (params_ptr->waveshape == TRIANGLE_MODE)){

			if((params_ptr->halfcycle == FIRST_HALFCYCLE && params_ptr->quadrant == FIRST_QUADRANT) || (params_ptr->halfcycle == SECOND_HALFCYCLE && params_ptr->quadrant == SECOND_QUADRANT)){

				if(symmetry_status == CW){

					symmetry_type_for_halfcycle = LENGTHEN;
				}
				else{

					symmetry_type_for_halfcycle = SHORTEN;
				}
			}
			else if((params_ptr->halfcycle == FIRST_HALFCYCLE && params_ptr->quadrant == SECOND_QUADRANT) || (params_ptr->halfcycle == SECOND_HALFCYCLE && params_ptr->quadrant == FIRST_QUADRANT)){

				if(symmetry_status == CW){

					symmetry_type_for_halfcycle = SHORTEN;
				}
				else{

					symmetry_type_for_halfcycle = LENGTHEN;
				}
			}
		}
		else if(params_ptr->waveshape == SQUARE_MODE){

			if(params_ptr->halfcycle == FIRST_HALFCYCLE){

				if(symmetry_status == CW){

					symmetry_type_for_halfcycle = LENGTHEN;
				}
				else{

					symmetry_type_for_halfcycle = SHORTEN;
				}
			}
			else{

				if(symmetry_status == CW){

					symmetry_type_for_halfcycle = SHORTEN;
				}
				else{

					symmetry_type_for_halfcycle = LENGTHEN;
				}
			}
		}

		if(symmetry_status == CW){

			pot_rotation_corrected = SYMMETRY_ADC_HALF_SCALE - 1 - params_ptr->symmetry;
		}
		else{ //CCW

			pot_rotation_corrected = SYMMETRY_ADC_HALF_SCALE - 1 - (SYMMETRY_ADC_FULL_SCALE - params_ptr->symmetry);
		}

		//HAVE TO BE uin16_t FOR 1ST AND 3RD VARIABLES HERE BECAUSE A uint8_t IS LIMITED TO 255!
		uint16_t two_fifty_six_minus_TIM16_raw_start_value = 256 - params_ptr->raw_start_value;

		uint16_t two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC = two_fifty_six_minus_TIM16_raw_start_value * pot_rotation_corrected;

		uint16_t two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC_and_shifted_by_ADC_bits = (uint16_t)(two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC >> SYMMETRY_ADC_NUM_BITS);


		//HAVE TO BE uin16_t HERE BECAUSE A uint8_t IS LIMITED TO 255!
		uint16_t manipulated_period_shorten = two_fifty_six_minus_TIM16_raw_start_value - two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC_and_shifted_by_ADC_bits; //manipulated shorten will always be less than 256

		uint16_t manipulated_period_lengthen = two_fifty_six_minus_TIM16_raw_start_value + two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC_and_shifted_by_ADC_bits; //manipulated lengthen can be greater than 256 up to 381


		if((manipulated_period_lengthen < 256) || ((manipulated_period_lengthen == 256) && (unsigned_bitwise_modulo(two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC, 8) == 0))){

			if(unsigned_bitwise_modulo(two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC, 8) == 128){

				//remainder is 128, which means two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC_and_shifted_by_ADC_bits ends in 0.5

				//manipulated_period_shorten should oscillate over the halfperiod between manipulated_period_shorten and manipulated_period_shorten - 1; //DONE
				//manipulated_period_lengthen should oscillate over the halfperiod between manipulated_period_lengthen and manipulated_period_lengthen + 1; //DONE

				if(symmetry_type_for_halfcycle == SHORTEN){

					TIM16_final_start_value_oscillation_mode = OSCILLATE_DOWNWARDS;
				}
				else if(symmetry_type_for_halfcycle == LENGTHEN){

					TIM16_final_start_value_oscillation_mode = OSCILLATE_UPWARDS;
				}
				params_ptr->prescaler_adjust = DO_NOTHING;
			}
			else if(unsigned_bitwise_modulo(two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC, 8) == 0){

				//remainder is zero, which means both shortened and lengthened manipulated periods have no no remainder

				manipulated_period_shorten = manipulated_period_shorten; //do nothing //DONE
				manipulated_period_lengthen = manipulated_period_lengthen; //do nothing //DONE

				if(symmetry_type_for_halfcycle == SHORTEN){
					TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
				}
				else if(symmetry_type_for_halfcycle == LENGTHEN){
					TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
				}
				params_ptr->prescaler_adjust = DO_NOTHING;
			}
			else if(unsigned_bitwise_modulo(two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC, 8) < 128){

				//remainder is less than 128, which means two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC_and_shifted_by_ADC_bits ends in less than 0.5

				manipulated_period_shorten = manipulated_period_shorten; //do nothing //DONE
				manipulated_period_lengthen = manipulated_period_lengthen; //do nothing //DONE

				if(symmetry_type_for_halfcycle == SHORTEN){
					TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
				}
				else if(symmetry_type_for_halfcycle == LENGTHEN){
					TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
				}
				params_ptr->prescaler_adjust = DO_NOTHING;
			}
			else if(unsigned_bitwise_modulo(two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC, 8) > 128){

				//remainder is greater than 128, which means two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC_and_shifted_by_ADC_bits ends in more than 0.5

				manipulated_period_shorten = manipulated_period_shorten - 1; //DONE
				manipulated_period_lengthen = manipulated_period_lengthen + 1; //DONE

				if(symmetry_type_for_halfcycle == SHORTEN){
					TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
				}
				else if(symmetry_type_for_halfcycle == LENGTHEN){
					TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
				}
				params_ptr->prescaler_adjust = DO_NOTHING;
			}
		}

		else if((manipulated_period_lengthen > 256) || ((manipulated_period_lengthen == 256) && (unsigned_bitwise_modulo(two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC, 8) > 0))){

			if(unsigned_bitwise_modulo(two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC, 8) == 128){

				manipulated_period_shorten = manipulated_period_shorten - 1; //DONE
				//NO NEED TO CHECK IF MANIPULATED_PERIOD_SHORTEN ENDS IN 0.5 AS IN THIS SPECIFIC CONDITION, WE HAVE ELIMINATED THAT POSSIBILITY
				manipulated_period_lengthen = manipulated_period_lengthen + 1; //DONE

				if(unsigned_bitwise_modulo(manipulated_period_lengthen, 1) == 0){

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//DO NOT OSCILLATE BETWEEN VALUES //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
				else{

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE
					//manipulated period_lengthened should oscillate over the halfperiod between manipulated_period_lengthen and manipulated_period_lengthen + 1. //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = OSCILLATE_UPWARDS;
						params_ptr->prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
			}
			else if(unsigned_bitwise_modulo(two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC, 8) == 0){

				manipulated_period_shorten = manipulated_period_shorten; //do nothing// //DONE

				if(unsigned_bitwise_modulo(manipulated_period_lengthen, 1) == 0){

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//DO NOT OSCILLATE BETWEEN VALUES //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
				else{

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE
					//manipulated period_lengthened should oscillate over the halfperiod between manipulated_period_lengthen and manipulated_period_lengthen + 1. //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = OSCILLATE_UPWARDS;
						params_ptr->prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
			}
			else if(unsigned_bitwise_modulo(two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC, 8) < 128){

				manipulated_period_shorten = manipulated_period_shorten; //do nothing// //DONE
				//NO NEED TO CHECK IF MANIPULATED_PERIOD_SHORTEN ENDS IN 0.5 AS IN THIS SPECIFIC CONDITION, WE HAVE ELIMINATED THAT POSSIBILITY
				manipulated_period_lengthen = manipulated_period_lengthen; //do nothing //DONE

				if(unsigned_bitwise_modulo(manipulated_period_lengthen, 1) == 0){

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//DO NOT OSCILLATE BETWEEN VALUES //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
				else{

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE
					//manipulated period_lengthened should oscillate over the halfperiod between manipulated_period_lengthen and manipulated_period_lengthen + 1. //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = OSCILLATE_UPWARDS;
						params_ptr->prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
			}
			else if(unsigned_bitwise_modulo(two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC, 8) > 128){

				manipulated_period_shorten = manipulated_period_shorten - 1; //DONE
				//NO NEED TO CHECK IF MANIPULATED_PERIOD_SHORTEN ENDS IN 0.5 AS IN THIS SPECIFIC CONDITION, WE HAVE ELIMINATED THAT POSSIBILITY
				manipulated_period_lengthen = manipulated_period_lengthen + 1; //DONE

				if(unsigned_bitwise_modulo(manipulated_period_lengthen, 1) == 0){

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//DO NOT OSCILLATE BETWEEN VALUES //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
				else{

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE
					//manipulated period_lengthened should oscillate over the halfperiod between manipulated_period_lengthen and manipulated_period_lengthen + 1. //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						params_ptr->prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = OSCILLATE_UPWARDS;
						params_ptr->prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
			}
		}

	//DETERMINE THE TIM16_FINAL_START_VALUE FROM MANIPULATED_PERIOD_LENGTHEN/SHORTEN CALCULATED
	//NOTE INVERSE RELATIONSHIP BETWEEN TIM16_FINAL_START_VALUE AND MANIPULATED_PERIOD_LENGTHEN/SHORTEN BECAUSE 256-TIM16_FINAL_START_VALUE = MANIPULATED_PERIOD_LENGTHEN/SHORTEN
	//ODD VALUES OF CURRENT_INDEX WILL FEATURE THE ADJUSTED(OSCILLATED) VALUE
	if(symmetry_type_for_halfcycle == SHORTEN){

		if(TIM16_final_start_value_oscillation_mode == DO_NOT_OSCILLATE){

			params_ptr->final_start_value = 256 - manipulated_period_shorten;
		}
		else if(TIM16_final_start_value_oscillation_mode == OSCILLATE_UPWARDS){

			if(unsigned_bitwise_modulo(params_ptr->index, 1) == 0){ //if current index is even

				params_ptr->final_start_value = 256 - manipulated_period_shorten;
			}
			else{ //if current index is odd

				params_ptr->final_start_value = 256 - manipulated_period_shorten - 1;
			}
		}
		else if(TIM16_final_start_value_oscillation_mode == OSCILLATE_DOWNWARDS){

			if(unsigned_bitwise_modulo(params_ptr->index, 1) == 0){ //if current index is even

				params_ptr->final_start_value = 256 - manipulated_period_shorten;
			}
			else{ //if current index is odd

				params_ptr->final_start_value = 256 - manipulated_period_shorten + 1;
			}
		}
	}
	else if(symmetry_type_for_halfcycle == LENGTHEN){

		if(TIM16_final_start_value_oscillation_mode == DO_NOT_OSCILLATE){

			params_ptr->final_start_value = 256 - manipulated_period_lengthen;
		}
		else if(TIM16_final_start_value_oscillation_mode == OSCILLATE_UPWARDS){

			if(unsigned_bitwise_modulo(params_ptr->index, 1) == 0){ //if current index is even

				params_ptr->final_start_value = 256 - manipulated_period_lengthen;
			}
			else{ //if current index is odd

				params_ptr->final_start_value = 256 - manipulated_period_lengthen - 1;
			}
		}
		else if(TIM16_final_start_value_oscillation_mode == OSCILLATE_DOWNWARDS){

			if(unsigned_bitwise_modulo(params_ptr->index, 1) == 0){ //if current index is even

				params_ptr->final_start_value = 256 - manipulated_period_lengthen;
			}
			else{ //if current index is odd

				params_ptr->final_start_value = 256 - manipulated_period_lengthen + 1;
			}
		}
	}

	Adjust_TIM16_Prescaler(params_ptr);

    #endif

    #if SYMMETRY_ON_OR_OFF == OFF
		params_ptr->final_start_value = params_ptr->raw_start_value;
		params_ptr->prescaler_adjust = DO_NOTHING;
        Adjust_TIM16_Prescaler(params_ptr);
    #endif

    return 1;
}

uint32_t unsigned_bitwise_modulo(uint32_t dividend, uint8_t base_2_exponent){

    return dividend & ((1 << base_2_exponent) - 1);
}

uint8_t Speed_Pot_Check(struct Params* params_ptr){

	if((tap_tempo_mode_is_active == YES) || (external_clock_mode_is_active == YES)){

		static uint16_t first_speed_measurement;

		static uint16_t second_speed_measurement;

		if(speed_pot_adc_measurement_num == 0){

			first_speed_measurement = params_ptr->speed;
			speed_pot_adc_measurement_num = 1;
		}
		else if(speed_pot_adc_measurement_num == 1){

			second_speed_measurement = params_ptr->speed;
			speed_pot_adc_measurement_num = 2;
		}
		else if(speed_pot_adc_measurement_num == 2){

			speed_pot_adc_measurement_num = 0;

			if(first_speed_measurement > second_speed_measurement){

				if(first_speed_measurement - second_speed_measurement > SPEED_TOLERANCE){

					tap_tempo_mode_is_active = NO;
					external_clock_mode_is_active = NO;

					//STOP SPEED POT CHECKING
					Stop_OC_TIM(&htim17, TIM_CHANNEL_1);
				}
			}
			else if(second_speed_measurement > first_speed_measurement){

				if(second_speed_measurement - first_speed_measurement > SPEED_TOLERANCE){

					tap_tempo_mode_is_active = NO;
					external_clock_mode_is_active = NO;

					//STOP SPEED POT CHECKING
					Stop_OC_TIM(&htim17, TIM_CHANNEL_1);
				}
			}
		}
	}
	return 1;
}

uint8_t Check_Tap_Tempo_Switch_State(enum Tap_Tempo_Switch_State *tap_tempo_switch_state_ptr){

	static uint8_t extend_rising_edge = 0;

	uint8_t switch_state = (uint8_t)HAL_GPIO_ReadPin(SW_IN_GPIO_Port, SW_IN_Pin);

	if(switch_state == 0){

		if(tap_tempo_switch_state_counter != 0){

			tap_tempo_switch_state_counter--;
		}
	}
	else{

		if(tap_tempo_switch_state_counter != TAP_TEMPO_SWITCH_CONFIDENCE_COUNT){

			if(extend_rising_edge == COUNT_TO_DELAY_RISING_TAP_TEMPO_EDGE){

				tap_tempo_switch_state_counter++;
				extend_rising_edge = 0;
			}
			else{
				extend_rising_edge++;
			}
		}
	}

	if(tap_tempo_switch_state_counter == 0){

		*tap_tempo_switch_state_ptr = DEPRESSED;
	}
	else if(tap_tempo_switch_state_counter == TAP_TEMPO_SWITCH_CONFIDENCE_COUNT){

		*tap_tempo_switch_state_ptr = NOT_DEPRESSED;
	}

	return 1;
}

uint8_t Start_UART_Receive(void){

	//START UART RECEIVE
	HAL_UART_Receive_DMA(&huart2, (uint8_t*)rx_buffer, sizeof(rx_buffer));

	return 1;
}

uint8_t Input_Capture_Processing(volatile uint16_t interrupt_period_value, struct Params* params_ptr){

	HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	input_capture_processing_can_be_started = NO; //reset flag

	//DETERMINE WHAT TO SET THE RAW_START_VALUE AND BASE_PRESCALER TO BASED ON THE I/P CAPTURE VALUE
	//CHECK FOR PRIMALITY
	if(isPrime(interrupt_period_value) == YES){

		interrupt_period_value += 1;
	}

	//START FINDING FACTORS
	uint32_t N = interrupt_period_value << 6; //calculate the N-value which is prescaler_meas * interrupt_period_meas. The measurement prescaler is used which is 64. (TIM2 has a prescaler of 64*512, but since we divide this value by 512, the prescaler is then just 64).

	for(uint8_t i = 0; i < 129; i++){ //check from period = 264 to 128 - there will be a prescaler for every non-prime value of N

		interrupt_period_value = 256 - i;
		uint16_t remainder = N % interrupt_period_value;

		if(remainder == 0){ //check if no remainder -> integer

			params_ptr->raw_prescaler = N / interrupt_period_value;
			break;
		}
	}

	params_ptr->raw_start_value = 256 - interrupt_period_value;

	Calculate_Next_Main_Oscillator_Values(params_ptr, (enum Next_Values_Processing_Mode)IP_CAPTURE_MODE);
	Process_TIM16_Final_Start_Value_and_Final_Prescaler(params_ptr);

	HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);

	return 1;
}

enum Validate isPrime(uint16_t x){
    for (int d = 2; d * d <= x; d++) {
        if (x % d == 0)
            return YES;
    }
    return NO;
}

uint8_t Calculate_Next_Main_Oscillator_Values(struct Params* params_ptr, enum Next_Values_Processing_Mode mode){

	if(mode == REGULAR_MODE){

		params_ptr->index++;

		if(params_ptr->index == FINAL_INDEX + 1){
			params_ptr->index = 0;
		}

		if(params_ptr->index == FIRST_QUADRANT_START_INDEX){
			params_ptr->quadrant = FIRST_QUADRANT;
			params_ptr->halfcycle = FIRST_HALFCYCLE;
		}
		else if(params_ptr->index == SECOND_QUADRANT_START_INDEX){
			params_ptr->quadrant = SECOND_QUADRANT;
			params_ptr->halfcycle = FIRST_HALFCYCLE;
		}
		else if(params_ptr->index == THIRD_QUADRANT_START_INDEX){
			params_ptr->quadrant = FIRST_QUADRANT;
			params_ptr->halfcycle = SECOND_HALFCYCLE;
		}
		else if(params_ptr->index == FOURTH_QUADRANT_START_INDEX){
			params_ptr->quadrant = SECOND_QUADRANT;
			params_ptr->halfcycle = SECOND_HALFCYCLE;
		}
	}
	else if(mode == IP_CAPTURE_MODE){

		if(params_ptr->waveshape == SINE_MODE || params_ptr->waveshape == TRIANGLE_MODE){

			params_ptr->index = SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX;
			params_ptr->quadrant = CURRENT_QUADRANT_SINE_OR_TRI_SYNCED;
			params_ptr->halfcycle = CURRENT_HALFCYCLE_SINE_OR_TRI_SYNCED;
		}
		else{

			params_ptr->index = SQUARE_WAVE_TEMPO_APEX_INDEX;
			params_ptr->quadrant = CURRENT_QUADRANT_SQUARE_SYNCED;
			params_ptr->halfcycle = CURRENT_HALFCYCLE_SQUARE_SYNCED;
		}
	}

	//ONCE INDEX IS SET, FIND THE DUTY VALUE
	if(params_ptr->waveshape == TRIANGLE_MODE){
		params_ptr->duty = tri_wavetable[params_ptr->index];
	}
	else if(params_ptr->waveshape == SINE_MODE){
		params_ptr->duty = sine_wavetable[params_ptr->index];
	}
	else if((params_ptr->waveshape == SQUARE_MODE) && (params_ptr->index < THIRD_QUADRANT_START_INDEX)){
		params_ptr->duty = PWM_DUTY_VALUE_MAX;
	}
	else if((params_ptr->waveshape == SQUARE_MODE) && (params_ptr->index >= THIRD_QUADRANT_START_INDEX)){
		params_ptr->duty = PWM_DUTY_VALUE_MIN;
	}

	//APPLY DEPTH
	#if DEPTH_ON_OR_OFF == 1

		//Apply Depth
		if(params_ptr->depth == ((1 << DEPTH_ADC_RESOLUTION) - 1)){ //255
			params_ptr->duty = PWM_DUTY_VALUE_MAX - params_ptr->duty;
		}
		else if(params_ptr->depth != 0){

			//duty = 1023 - duty*(current_depth >> 8);
			uint32_t multiply_product = 0;
			multiply_product = (params_ptr->duty) * (params_ptr->depth);
			params_ptr->duty = PWM_DUTY_VALUE_MAX - (multiply_product >> 8);
		}
		else{
			params_ptr->duty = PWM_DUTY_VALUE_MAX; //if depth is 0, just output 1023
		}

	#endif

	//SET THE NEXT VALUE FOR THE MAIN OSCILLATOR
	params_ptr->prev_duty = params_ptr->duty;

	return 1;
}

uint8_t Write_Next_Main_Oscillator_Values_to_Delay_Line(struct Params* params_ptr, struct Delay_Line* delay_line_ptr){

	//STORE THE VALUES IN THE APPROPRIATE '0TH - 1' INDEX RELATIVE TO THE START POINTER
		if(delay_line_ptr->duty_delay_line_start_offset != 0){
			delay_line_ptr->duty_delay_line_storage_array[delay_line_ptr->duty_delay_line_start_offset - 1] = params_ptr->duty;
		}
		else{
			delay_line_ptr->duty_delay_line_storage_array[FINAL_INDEX + 1] = params_ptr->duty;
		}

		//DECREMENT THE START AND FINISH POINTERS
		if(delay_line_ptr->duty_delay_line_start_offset == 0){
			delay_line_ptr->duty_delay_line_start_offset = FINAL_INDEX + 1;
			delay_line_ptr->duty_delay_line_finish_offset = delay_line_ptr->duty_delay_line_finish_offset - 1;
		}
		else if(delay_line_ptr->duty_delay_line_finish_offset == 0){
			delay_line_ptr->duty_delay_line_finish_offset = FINAL_INDEX + 1;
			delay_line_ptr->duty_delay_line_start_offset = delay_line_ptr->duty_delay_line_start_offset - 1;
		}
		else{
			delay_line_ptr->duty_delay_line_start_offset = delay_line_ptr->duty_delay_line_start_offset - 1;
			delay_line_ptr->duty_delay_line_finish_offset = delay_line_ptr->duty_delay_line_finish_offset - 1;
		}

		//DETERMINE THE DELAYED WAVE'S VALUES
		if(delay_line_ptr->duty_delay_line_start_offset + delay_line_ptr->duty_delay_line_read_pointer_offset > FINAL_INDEX + 1){ //if the desired starting index falls off the end of the array
			params_ptr->duty_delayed = *(delay_line_ptr->duty_delay_line_storage_array + (delay_line_ptr->duty_delay_line_start_offset + delay_line_ptr->duty_delay_line_read_pointer_offset - (FINAL_INDEX + 1)));
		}
		else{
			params_ptr->duty_delayed = *(delay_line_ptr->duty_delay_line_storage_array + delay_line_ptr->duty_delay_line_start_offset + delay_line_ptr->duty_delay_line_read_pointer_offset);
		}

	return 1;
}

uint8_t Process_ADC_Conversion_Values(struct Params* params_ptr, struct Delay_Line* delay_line_ptr, volatile uint16_t* ADCResultsDMA_ptr){

	//GET WAVESHAPE
	uint16_t ADC_result = ADCResultsDMA_ptr[WAVESHAPE_ADC_RESULT_INDEX]; //set ADC_Result to waveshape index value

	if(ADC_result <= TRIANGLE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = TRIANGLE_MODE; //triangle wave
	}
	else if (ADC_result <= SINE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = SINE_MODE; //sine wave
	}
	else if (ADC_result <= SQUARE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = SQUARE_MODE; //square wave
	}

	//GET SPEED
	params_ptr->speed = ADCResultsDMA_ptr[SPEED_ADC_RESULT_INDEX] >> 2; //convert to 10-bit

	//GET DEPTH
	#if DEPTH_ON_OR_OFF == ON

		params_ptr->depth = ADCResultsDMA_ptr[DEPTH_ADC_RESULT_INDEX] >> 4; //convert to 8-bit

	#endif

	//GET SYMMETRY
	#if SYMMETRY_ON_OR_OFF == ON

		params_ptr->symmetry = ADCResultsDMA_ptr[SYMMETRY_ADC_RESULT_INDEX] >> 4; //convert to 8-bit

	#endif

	//GET DELAY LINE READ POINTER OFFSET
	delay_line_ptr->duty_delay_line_read_pointer_offset = ADCResultsDMA_ptr[DUTY_DELAY_LINE_READ_POINTER_OFFSET_ADC_RESULT_INDEX] >> 3; //convert to 9-bit

	return 1;
}

uint8_t Set_Oscillator_Values(struct Params* params_ptr){

	////////////////////////////////////////////////////////
	//SET THE CURRENT(prev) VALUES FOR THE MAIN OSCILLATOR//
	////////////////////////////////////////////////////////
	TIM16->EGR |= TIM_EGR_UG; //DO NOT DELETE THIS LINE, IT LITERALLY MAKES OR BREAKS THE BASTARD - It triggers an 'update' event
	__HAL_TIM_SET_COUNTER(&htim16, params_ptr->final_start_value); //this line must go here, or at least very near the beginning!
	__HAL_TIM_SET_PRESCALER(&htim16, (params_ptr->final_prescaler - 1)); //have to take one off the divisor
	TIM1->EGR |= TIM_EGR_UG; //not sure if we really need this line but gonna keep it here because it worked wonders for TIM16
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, params_ptr->prev_duty); //updates the CCR register of TIM14, which sets duty, i.e. the ON time relative to the total period which is set by the ARR.

	/////////////////////////////////////////////////////////////
	//SET THE CURRENT(prev) VALUES FOR THE SECONDARY OSCILLATOR//
	/////////////////////////////////////////////////////////////
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, params_ptr->duty_delayed); //updates the CCR register of TIM14, which sets duty, i.e. the ON time relative to the total period which is set by the ARR.

	return 1;
}



