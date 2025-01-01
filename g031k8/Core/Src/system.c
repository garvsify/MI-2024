//INCLUDES
#include "system.h"

//CONSTANTS
const uint16_t sine_wavetable[512]={512,518,524,530,537,543,549,555,562,568,574,580,587,593,599,605,611,617,624,630,636,642,648,654,660,666,672,678,684,690,696,701,707,713,719,725,730,736,741,747,753,758,764,769,774,780,785,790,796,801,806,811,816,821,826,831,836,841,846,850,855,860,864,869,873,878,882,886,890,895,899,903,907,911,915,919,922,926,930,933,937,940,944,947,950,953,957,960,963,966,968,971,974,977,979,982,984,986,989,991,993,995,997,999,1001,1003,1004,1006,1008,1009,1011,1012,1013,1014,1015,1017,1017,1018,1019,1020,1021,1021,1022,1022,1022,1023,1023,1023,1023,1023,1023,1023,1022,1022,1022,1021,1021,1020,1019,1018,1017,1017,1015,1014,1013,1012,1011,1009,1008,1006,1004,1003,1001,999,997,995,993,991,989,986,984,982,979,977,974,971,968,966,963,960,957,953,950,947,944,940,937,933,930,926,922,919,915,911,907,903,899,895,890,886,882,878,873,869,864,860,855,850,846,841,836,831,826,821,816,811,806,801,796,790,785,780,774,769,764,758,753,747,741,736,730,725,719,713,707,701,696,690,684,678,672,666,660,654,648,642,636,630,624,617,611,605,599,593,587,580,574,568,562,555,549,543,537,530,524,518,512,505,499,493,486,480,474,468,461,455,449,443,436,430,424,418,412,406,399,393,387,381,375,369,363,357,351,345,339,333,327,322,316,310,304,298,293,287,282,276,270,265,259,254,249,243,238,233,227,222,217,212,207,202,197,192,187,182,177,173,168,163,159,154,150,145,141,137,133,128,124,120,116,112,108,104,101,97,93,90,86,83,79,76,73,70,66,63,60,57,55,52,49,46,44,41,39,37,34,32,30,28,26,24,22,20,19,17,15,14,12,11,10,9,8,6,6,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,6,6,8,9,10,11,12,14,15,17,19,20,22,24,26,28,30,32,34,37,39,41,44,46,49,52,55,57,60,63,66,70,73,76,79,83,86,90,93,97,101,104,108,112,116,120,124,128,133,137,141,145,150,154,159,163,168,173,177,182,187,192,197,202,207,212,217,222,227,233,238,243,249,254,259,265,270,276,282,287,293,298,304,310,316,322,327,333,339,345,351,357,363,369,375,381,387,393,399,406,412,418,424,430,436,443,449,455,461,468,474,480,486,493,499,505};
const uint16_t tri_wavetable[512]={512,516,520,524,528,532,536,540,544,548,552,556,560,564,568,572,576,580,584,588,592,596,600,604,608,612,616,620,624,628,632,636,640,644,648,652,656,660,664,668,672,676,680,684,688,692,696,700,704,708,712,716,720,724,728,732,736,740,744,748,752,756,760,764,768,771,775,779,783,787,791,795,799,803,807,811,815,819,823,827,831,835,839,843,847,851,855,859,863,867,871,875,879,883,887,891,895,899,903,907,911,915,919,923,927,931,935,939,943,947,951,955,959,963,967,971,975,979,983,987,991,995,999,1003,1007,1011,1015,1019,1023,1019,1015,1011,1007,1003,999,995,991,987,983,979,975,971,967,963,959,955,951,947,943,939,935,931,927,923,919,915,911,907,903,899,895,891,887,883,879,875,871,867,863,859,855,851,847,843,839,835,831,827,823,819,815,811,807,803,799,795,791,787,783,779,775,771,767,763,759,755,751,747,743,739,735,731,727,723,719,715,711,707,703,699,695,691,687,683,679,675,671,667,663,659,655,651,647,643,639,635,631,627,623,619,615,611,607,603,599,595,591,587,583,579,575,571,567,563,559,555,551,547,543,539,535,531,527,523,519,515,512,508,504,500,496,492,488,484,480,476,472,468,464,460,456,452,448,444,440,436,432,428,424,420,416,412,408,404,400,396,392,388,384,380,376,372,368,364,360,356,352,348,344,340,336,332,328,324,320,316,312,308,304,300,296,292,288,284,280,276,272,268,264,260,256,252,248,244,240,236,232,228,224,220,216,212,208,204,200,196,192,188,184,180,176,172,168,164,160,156,152,148,144,140,136,132,128,124,120,116,112,108,104,100,96,92,88,84,80,76,72,68,64,60,56,52,48,44,40,36,32,28,24,20,16,12,8,4,0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,236,240,244,248,252,256,260,264,268,272,276,280,284,288,292,296,300,304,308,312,316,320,324,328,332,336,340,344,348,352,356,360,364,368,372,376,380,384,388,392,396,400,404,408,412,416,420,424,428,432,436,440,444,448,452,456,460,464,468,472,476,480,484,488,492,496,500,504,508};
const uint16_t TIM16_prescaler_divisors[6] = {2048, 1024, 512, 256, 128, 64}; //2048 is used only by base prescaler index == 1 during symmetry adjustment
const uint8_t num_ADC_conversions = sizeof(ADCResultsDMA) / sizeof(ADCResultsDMA[0]);

//VARIABLE DEFINITIONS
volatile uint8_t current_waveshape = 0;
volatile uint16_t current_speed = 0;
volatile uint16_t current_depth = 0;
volatile uint32_t current_symmetry = 0;
volatile uint16_t current_index = 0;
volatile uint8_t current_halfcycle = 0;
volatile uint8_t current_quadrant = 0;
volatile uint16_t TIM16_final_start_value = 0;
volatile enum Adjust_Prescaler_Action TIM16_prescaler_adjust = 0;
volatile uint16_t TIM16_raw_start_value = 0;
volatile uint8_t TIM16_base_prescaler_divisors_index = 0;
volatile uint16_t duty = 0;
volatile uint8_t TIM16_prescaler_divisors_final_index = 0;
volatile uint8_t TIM16_prescaler_divisors_final_index_locked = 0;
volatile uint16_t ADCResultsDMA[4] = {0};
volatile enum Validate initial_ADC_conversion_complete = NO;
volatile enum Validate TIM16_callback_active = NO;
volatile uint16_t TIM16_final_start_value_locked = 0;
volatile uint8_t TIM16_prescaler_adjust_locked = 0;
volatile uint16_t prev_duty = 0;
volatile enum Input_Capture_Event input_capture_event = FIRST;
volatile uint32_t TIM2_ch1_input_capture_value;
volatile enum Validate input_capture_measurement_is_ongoing = NO;
volatile enum Validate speed_pot_is_disabled = NO;
volatile enum Validate input_capture_measurement_reelapse_1_is_ongoing = NO;
volatile enum Validate input_capture_measurement_reelapse_2_is_ongoing = NO;
volatile uint16_t interrupt_period = 0;
volatile enum Adjust_Prescaler_Action TIM16_prescaler_adjust_to_be_loaded = 0;
volatile uint16_t TIM16_raw_start_value_to_be_loaded = 0;
volatile uint16_t TIM16_final_start_value_to_be_loaded = 0;
volatile uint8_t TIM16_base_prescaler_divisors_index_to_be_loaded = 0;
volatile uint16_t duty_to_be_loaded = 0;
volatile uint8_t TIM16_prescaler_divisors_final_index_to_be_loaded = 0;
volatile uint8_t current_halfcycle_to_be_loaded = 0;
volatile uint8_t current_quadrant_to_be_loaded = 0;
volatile uint16_t current_index_to_be_loaded = 0;
volatile uint16_t current_depth_to_be_loaded = 0;
volatile uint16_t duty_delay_line[512] = {0};
volatile uint8_t prescaler_divisors_final_index_delay_line[512] = {0};
volatile uint16_t final_start_value_delay_line[512] = {0};
volatile uint16_t delay_line_read_pointer_offset = 0;
volatile uint16_t final_start_value_delayed = 0;
volatile uint8_t prescaler_divisors_final_index_delayed = 0;
volatile uint16_t duty_delayed = 0;

//FUNCTION DEFINITIONS
uint8_t Global_Interrupt_Enable(void){

	__enable_irq();
	return 1;
}

uint8_t Global_Interrupt_Disable(void){

	__disable_irq();
	return 1;
}

uint8_t Start_ADC_Trig_Timer(void)
{
	uint8_t ok = Start_OC_TIM(&htim17, TIM_CHANNEL_1); //start adc trig.

	if(ok != HAL_OK){

			Error_Handler();
	}

	return ok;
}

uint8_t Start_PWM_Gen_Timer_Main_Oscillator(void)
{
	uint8_t ok = Start_PWM_TIM(&htim14, TIM_CHANNEL_1); //start PWM

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Start_PWM_Gen_Timer_Secondary_Oscillator(void)
{
	uint8_t ok = Start_PWM_TIM(&htim14, TIM_CHANNEL_1); //start PWM

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Start_Freq_Gen_Timer(void)
{
	uint8_t ok = Start_OC_TIM(&htim16, TIM_CHANNEL_1); //start freq. gen.

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Start_PWM_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel){

	uint8_t ok_AND = 0;
	ok_AND = HAL_TIM_Base_Start(TIM);
	ok_AND &= HAL_TIM_PWM_Start(TIM, PWM_TIM_channel);

	if(ok_AND != HAL_OK){

		Error_Handler();
	}

	return ok_AND;
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

uint8_t Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(void){

	uint32_t speed_control = 0;
	uint8_t how_many_128 = 0;

    speed_control = current_speed * NUMBER_OF_FREQUENCY_STEPS;
    speed_control = speed_control >> 10;

    //speed_control = (speed_adc_10_bit/1024)*883

        if(speed_control <= (127-12)){ //inequality is correct!

            TIM16_raw_start_value = (uint8_t) speed_control + 12;
            TIM16_base_prescaler_divisors_index = SLOWEST_SPEED_PRESCALER_DIVISORS_ARRAY_INDEX;
        }
        else{ 	//(speed_control > (127-12))

            uint16_t speed_control_subtracted;
            speed_control_subtracted = speed_control - (127-12);
            how_many_128 = (uint8_t)(speed_control_subtracted >> 7); //divide by 128, i.e. return how many 128s go into the speed_control
            TIM16_raw_start_value = (uint8_t)(speed_control_subtracted - (uint16_t)(how_many_128 << 7)); //how_many_128*128, set TMR0
            //biggest how_many_128 for NUMBER_OF_FREQUENCY_STEPS == 600 is 3
            //biggest base_prescaler_divisors_index == 5 for NUMBER_OF_FREQUENCY_STEPS == 600
            TIM16_base_prescaler_divisors_index = (uint8_t)(how_many_128 + SLOWEST_SPEED_PRESCALER_DIVISORS_ARRAY_INDEX + 1);
        }
    return 1;
}

uint8_t Adjust_TIM16_Prescaler(uint8_t TIM16_prescaler_adjust_arg){

    if(TIM16_prescaler_adjust_arg == MULTIPLY_BY_TWO){
    	TIM16_prescaler_divisors_final_index = TIM16_base_prescaler_divisors_index - 1;
    }
    else if(TIM16_prescaler_adjust_arg == DO_NOTHING){
    	TIM16_prescaler_divisors_final_index = TIM16_base_prescaler_divisors_index;
    }
    return 1;
}

uint8_t Process_TIM16_Final_Start_Value_and_Prescaler_Adjust(void){

    #if SYMMETRY_ON_OR_OFF == ON

		volatile enum TIM16_final_start_value_Oscillation_Mode TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
		volatile enum Symmetry_Type symmetry_type_for_halfcycle = SHORTEN;

		volatile uint8_t pot_rotation_corrected = 0;
		volatile uint8_t symmetry_status = CW;

		if(current_symmetry < SYMMETRY_ADC_HALF_SCALE){ //adc = 0-127
			symmetry_status = CW;
		}
		else{ //adc is 128-255
			symmetry_status = CCW;
		}

		if((current_waveshape == SINE_MODE) || (current_waveshape == TRIANGLE_MODE)){

			if((current_halfcycle == FIRST_HALFCYCLE && current_quadrant == FIRST_QUADRANT) || (current_halfcycle == SECOND_HALFCYCLE && current_quadrant == SECOND_QUADRANT)){

				if(symmetry_status == CW){

					symmetry_type_for_halfcycle = LENGTHEN;
				}
				else{

					symmetry_type_for_halfcycle = SHORTEN;
				}
			}
			else if((current_halfcycle == FIRST_HALFCYCLE && current_quadrant == SECOND_QUADRANT) || (current_halfcycle == SECOND_HALFCYCLE && current_quadrant == FIRST_QUADRANT)){

				if(symmetry_status == CW){

					symmetry_type_for_halfcycle = SHORTEN;
				}
				else{

					symmetry_type_for_halfcycle = LENGTHEN;
				}
			}
		}
		else if(current_waveshape == SQUARE_MODE){

			if(current_halfcycle == FIRST_HALFCYCLE){

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

			pot_rotation_corrected = SYMMETRY_ADC_HALF_SCALE - 1 - current_symmetry;
		}
		else{ //CCW

			pot_rotation_corrected = SYMMETRY_ADC_HALF_SCALE - 1 - (SYMMETRY_ADC_FULL_SCALE - current_symmetry);
		}

		//HAVE TO BE uin16_t FOR 1ST AND 3RD VARIABLES HERE BECAUSE A uint8_t IS LIMITED TO 255!
		uint16_t two_fifty_six_minus_TIM16_raw_start_value = 256 - TIM16_raw_start_value;

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
				TIM16_prescaler_adjust = DO_NOTHING;
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
				TIM16_prescaler_adjust = DO_NOTHING;
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
				TIM16_prescaler_adjust = DO_NOTHING;
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
				TIM16_prescaler_adjust = DO_NOTHING;
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
						TIM16_prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						TIM16_prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
				else{

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE
					//manipulated period_lengthened should oscillate over the halfperiod between manipulated_period_lengthen and manipulated_period_lengthen + 1. //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						TIM16_prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = OSCILLATE_UPWARDS;
						TIM16_prescaler_adjust = MULTIPLY_BY_TWO;
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
						TIM16_prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						TIM16_prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
				else{

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE
					//manipulated period_lengthened should oscillate over the halfperiod between manipulated_period_lengthen and manipulated_period_lengthen + 1. //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						TIM16_prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = OSCILLATE_UPWARDS;
						TIM16_prescaler_adjust = MULTIPLY_BY_TWO;
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
						TIM16_prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						TIM16_prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
				else{

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE
					//manipulated period_lengthened should oscillate over the halfperiod between manipulated_period_lengthen and manipulated_period_lengthen + 1. //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						TIM16_prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = OSCILLATE_UPWARDS;
						TIM16_prescaler_adjust = MULTIPLY_BY_TWO;
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
						TIM16_prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						TIM16_prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
				else{

					manipulated_period_lengthen = manipulated_period_lengthen >> 1; //DONE
					//prescaler during lengthened halfperiod should be set to half //DONE
					//manipulated period_lengthened should oscillate over the halfperiod between manipulated_period_lengthen and manipulated_period_lengthen + 1. //DONE

					if(symmetry_type_for_halfcycle == SHORTEN){
						TIM16_final_start_value_oscillation_mode = DO_NOT_OSCILLATE;
						TIM16_prescaler_adjust = DO_NOTHING;
					}
					else if(symmetry_type_for_halfcycle == LENGTHEN){
						TIM16_final_start_value_oscillation_mode = OSCILLATE_UPWARDS;
						TIM16_prescaler_adjust = MULTIPLY_BY_TWO;
					}
				}
			}
		}

	//DETERMINE THE TIM16_FINAL_START_VALUE FROM MANIPULATED_PERIOD_LENGTHEN/SHORTEN CALCULATED
	//NOTE INVERSE RELATIONSHIP BETWEEN TIM16_FINAL_START_VALUE AND MANIPULATED_PERIOD_LENGTHEN/SHORTEN BECAUSE 256-TIM16_FINAL_START_VALUE = MANIPULATED_PERIOD_LENGTHEN/SHORTEN
	//ODD VALUES OF CURRENT_INDEX WILL FEATURE THE ADJUSTED(OSCILLATED) VALUE
	if(symmetry_type_for_halfcycle == SHORTEN){

		if(TIM16_final_start_value_oscillation_mode == DO_NOT_OSCILLATE){

			TIM16_final_start_value = 256 - manipulated_period_shorten;
		}
		else if(TIM16_final_start_value_oscillation_mode == OSCILLATE_UPWARDS){

			if(unsigned_bitwise_modulo(current_index, 1) == 0){ //if current index is even

				TIM16_final_start_value = 256 - manipulated_period_shorten;
			}
			else{ //if current index is odd

				TIM16_final_start_value = 256 - manipulated_period_shorten - 1;
			}
		}
		else if(TIM16_final_start_value_oscillation_mode == OSCILLATE_DOWNWARDS){

			if(unsigned_bitwise_modulo(current_index, 1) == 0){ //if current index is even

				TIM16_final_start_value = 256 - manipulated_period_shorten;
			}
			else{ //if current index is odd

				TIM16_final_start_value = 256 - manipulated_period_shorten + 1;
			}
		}
	}
	else if(symmetry_type_for_halfcycle == LENGTHEN){

		if(TIM16_final_start_value_oscillation_mode == DO_NOT_OSCILLATE){

			TIM16_final_start_value = 256 - manipulated_period_lengthen;
		}
		else if(TIM16_final_start_value_oscillation_mode == OSCILLATE_UPWARDS){

			if(unsigned_bitwise_modulo(current_index, 1) == 0){ //if current index is even

				TIM16_final_start_value = 256 - manipulated_period_lengthen;
			}
			else{ //if current index is odd

				TIM16_final_start_value = 256 - manipulated_period_lengthen - 1;
			}
		}
		else if(TIM16_final_start_value_oscillation_mode == OSCILLATE_DOWNWARDS){

			if(unsigned_bitwise_modulo(current_index, 1) == 0){ //if current index is even

				TIM16_final_start_value = 256 - manipulated_period_lengthen;
			}
			else{ //if current index is odd

				TIM16_final_start_value = 256 - manipulated_period_lengthen + 1;
			}
		}
	}

	Adjust_TIM16_Prescaler(TIM16_prescaler_adjust);

    #endif

    #if SYMMETRY_ON_OR_OFF == OFF
        TIM16_final_start_value = TIM16_raw_start_value;
        TIM16_prescaler_adjust = DO_NOTHING;
        Adjust_TIM16_Prescaler(TIM16_prescaler_adjust);
    #endif

    return 1;
}

uint32_t unsigned_bitwise_modulo(uint32_t dividend, uint8_t base_2_exponent){

    return dividend & ((1 << base_2_exponent) - 1);
}

