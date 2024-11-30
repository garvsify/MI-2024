//INCLUDES
#include "system.h"

//CONSTANTS
const uint16_t sine_table_one_quadrant[129]={512,518,524,530,537,543,549,555,562,568,574,580,587,593,599,605,611,617,624,630,636,642,648,654,660,666,672,678,684,690,696,701,707,713,719,725,730,736,741,747,753,758,764,769,774,780,785,790,796,801,806,811,816,821,826,831,836,841,846,850,855,860,864,869,873,878,882,886,890,895,899,903,907,911,915,919,922,926,930,933,937,940,944,947,950,953,957,960,963,966,968,971,974,977,979,982,984,986,989,991,993,995,997,999,1001,1003,1004,1006,1008,1009,1011,1012,1013,1014,1015,1017,1017,1018,1019,1020,1021,1021,1022,1022,1022,1023,1023,1023,1023};
const uint16_t tri_table_one_quadrant[129]={512,516,520,524,528,532,536,540,544,548,552,556,560,564,568,572,576,580,584,588,592,596,600,604,608,612,616,620,624,628,632,636,640,644,648,652,656,660,664,668,672,676,680,684,688,692,696,700,704,708,712,716,720,724,728,732,736,740,744,748,752,756,760,763,767,771,775,779,783,787,791,795,799,803,807,811,815,819,823,827,831,835,839,843,847,851,855,859,863,867,871,875,879,883,887,891,895,899,903,907,911,915,919,923,927,931,935,939,943,947,951,955,959,963,967,971,975,979,983,987,991,995,999,1003,1007,1011,1015,1019,1023};
//const uint16_t TIM16_prescaler_divisors[9] = {2048, 1024, 512, 256, 128, 64, 32, 16, 8};
const uint16_t TIM16_prescaler_divisors[9] = {4096, 2048, 1024, 512, 256, 128, 64, 32, 16};
const uint8_t num_ADC_conversions = sizeof(ADCResultsDMA) / sizeof(ADCResultsDMA[0]);

//VARIABLE DEFINITIONS
volatile uint8_t current_waveshape = 0;
volatile uint16_t current_speed_linear = 0;
volatile uint32_t current_speed_linear_32 = 0;
volatile uint16_t current_depth = 0;
volatile uint32_t current_symmetry = 0;
volatile uint8_t current_one_quadrant_index = 0;
volatile uint8_t current_halfcycle = 0;
volatile uint8_t current_quadrant = 0;
volatile uint32_t TIM16_final_start_value = 0;
volatile uint8_t TIM16_prescaler_adjust = 0;
volatile uint32_t TIM16_raw_start_value = 0;
volatile uint8_t TIM16_base_prescaler_divisors_index = 0;
volatile uint16_t duty = 0;
volatile uint8_t TIM16_prescaler_overflow_flag = 0;
volatile uint8_t TIM16_prescaler_divisors_final_index = 0;
volatile uint16_t ADCResultsDMA[4] = {0};
volatile uint8_t initial_ADC_conversion_complete = 0;
volatile uint8_t processing_TIM16_final_start_value_and_prescaler = NO;
volatile uint8_t TIM16_callback_active = NO;
volatile uint32_t exit_TIM16_final_start_value_locked = 0;
volatile uint8_t exit_TIM16_prescaler_adjust_locked = 0;
volatile uint8_t halfcycle_has_changed = NO;
volatile uint8_t halfcycle_is_about_to_change = NO;
volatile uint32_t TIM16_final_start_value_locked = 0;
volatile uint8_t TIM16_prescaler_adjust_locked = 0;
volatile uint16_t prev_duty = 0;

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

uint8_t Start_PWM_Gen_Timer(void)
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

	uint8_t ok = 0;
	ok = HAL_TIM_Base_Start(TIM);
	ok = HAL_TIM_PWM_Start(TIM, PWM_TIM_channel);

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

uint8_t Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(void){

	uint16_t speed_control = 0;
	uint32_t speed_control_32 = 0;
	uint8_t how_many_128 = 0;

    current_speed_linear_32 = current_speed_linear;
    speed_control_32 = current_speed_linear_32 * NUMBER_OF_FREQUENCY_STEPS;
    speed_control_32 = speed_control_32 >> 10;
    speed_control = (uint16_t) speed_control_32;
    //speed_control = (speed_adc_10_bit/1024)*883
        if(speed_control <= (127-12)){ //inequality is correct!
            TIM16_raw_start_value = (uint8_t) speed_control + 12;
            TIM16_base_prescaler_divisors_index = 1;
        }
        else{ 	//(speed_control > (127-12))
            uint16_t speed_control_subtracted;
            speed_control_subtracted = speed_control - (127-12);
            how_many_128 = (uint8_t)(speed_control_subtracted >> 7); //divide by 128, i.e. return how many 128s go into the speed_control
            TIM16_raw_start_value = (uint8_t)(speed_control_subtracted - (uint16_t)(how_many_128 << 7)); //how_many_128*128, set TMR0
            //biggest how_many_128 for NUMBER_OF_FREQUENCY_STEPS == 600 is 3
            //biggest base_prescaler_divisors_index == 5 for NUMBER_OF_FREQUENCY_STEPS == 600
            TIM16_base_prescaler_divisors_index = (uint8_t)(how_many_128 + 2);
        }
    return 1;
}


uint8_t Adjust_and_Set_TIM16_Prescaler(uint8_t TIM16_prescaler_adjust_arg){

    if(TIM16_prescaler_adjust_arg == DIVIDE_BY_TWO){
        TIM16_prescaler_divisors_final_index = TIM16_base_prescaler_divisors_index + 1;
    }
    else if(TIM16_prescaler_adjust_arg == DIVIDE_BY_FOUR){
    	TIM16_prescaler_divisors_final_index= TIM16_base_prescaler_divisors_index + 2;
    }
    else if(TIM16_prescaler_adjust_arg == MULTIPLY_BY_TWO){
    	TIM16_prescaler_divisors_final_index = TIM16_base_prescaler_divisors_index - 1;
    }
    else if(TIM16_prescaler_adjust_arg == DO_NOTHING){
    	TIM16_prescaler_divisors_final_index = TIM16_base_prescaler_divisors_index;
    }
    __HAL_TIM_SET_PRESCALER(&htim16, (TIM16_prescaler_divisors[TIM16_prescaler_divisors_final_index]) - 1); //have to take one off the divisor
    return 1;
}


#if SYMMETRY_ON_OR_OFF == 1

    uint8_t Shorten_Period(void){
        #if SYMMETRY_ADC_RESOLUTION == 8
            uint32_t twofiftysix_minus_start_value_final = (((256-TIM16_raw_start_value)*(SHORTEN_POWER_OF_TWO_CONSTANT_8_BIT_SYM+(24*current_symmetry)))>>SHORTEN_POWER_OF_TWO_DIVISOR_8_BIT_SYM);
        #endif
        #if SYMMETRY_ADC_RESOLUTION == 10
            uint32_t twofiftysix_minus_start_value_final = (((256-TIM16_raw_start_value)*(SHORTEN_POWER_OF_TWO_CONSTANT_10_BIT_SYM+(24*current_symmetry)))>>SHORTEN_POWER_OF_TWO_DIVISOR_10_BIT_SYM);
        #endif

        TIM16_final_start_value = (256-twofiftysix_minus_start_value_final);
        TIM16_prescaler_adjust = DO_NOTHING;
        return 1;
    }

    uint8_t Lengthen_Period(void){
        #if SYMMETRY_ADC_RESOLUTION == 8
            uint32_t twofiftysix_minus_start_value_final = (((256-TIM16_raw_start_value)*(LENGTHEN_CONSTANT_8_BIT_SYM-(3*current_symmetry)))>>LENGTHEN_POWER_OF_TWO_DIVISOR_8_BIT_SYM);
        #endif
        #if SYMMETRY_ADC_RESOLUTION == 10
            uint32_t twofiftysix_minus_start_value_final = (((256-TIM16_raw_start_value)*(LENGTHEN_CONSTANT_10_BIT_SYM-(3*current_symmetry)))>>LENGTHEN_POWER_OF_TWO_DIVISOR_10_BIT_SYM);
        #endif

        if(twofiftysix_minus_start_value_final > 256){
            twofiftysix_minus_start_value_final = (twofiftysix_minus_start_value_final >> 1);
            TIM16_final_start_value = (256-twofiftysix_minus_start_value_final);
            TIM16_prescaler_adjust = MULTIPLY_BY_TWO;
        }
        else{
            TIM16_final_start_value = 256-twofiftysix_minus_start_value_final;
            TIM16_prescaler_adjust = DO_NOTHING;
        }
        return 1;
    }
#endif


uint8_t Process_TIM16_Final_Start_Value_and_Prescaler_Adjust(void){

    #if SYMMETRY_ON_OR_OFF == ON
        if(current_symmetry == SYMMETRY_ADC_HALF_SCALE){
            TIM16_final_start_value = TIM16_raw_start_value;
            TIM16_prescaler_adjust = DO_NOTHING;
        }
        else{
            uint8_t symmetry_status = CCW;
            if(current_symmetry > SYMMETRY_ADC_HALF_SCALE){
                current_symmetry = SYMMETRY_ADC_FULL_SCALE - current_symmetry; //converts current_symmetry to 128 -> 0 range (same range as CCW regime, more or less)
                symmetry_status = CW;
            }
            if(current_halfcycle == FIRST_HALFCYCLE){
                if(symmetry_status == CCW){
                    Shorten_Period();
                }
                else{
                    Lengthen_Period();
                }
            }
            else if(current_halfcycle == SECOND_HALFCYCLE){
                if(symmetry_status == CCW){
                    Lengthen_Period();
                }
                else{
                    Shorten_Period();
                }
            }
        }

        //Adjust_and_Set_TIM16_Prescaler(); //DO NOT COMMENT BACK IN

        //DONT NEED AS PRESCALERS ARE MUCH BIGGER NOW - DO NOT COMMENT BACK IN
        /*//Adjust TMR0 for 2 instruction tick delay on update (for low prescaler values)
        if(TIM16_prescaler_divisors_final_index == 8){//prescaler is 1:1
            TIM16_final_start_value = TIM16_final_start_value + 2; //(256-TMR0_final) needs to be 2 counts less
        }
        else if(TIM16_prescaler_divisors_final_index == 7){//prescaler is 2:1
            TIM16_final_start_value = TIM16_final_start_value + 1; //(256-TMR0_final) needs to be 1 counts less
        }*/

    #endif

    #if SYMMETRY_ON_OR_OFF == OFF
        TIM16_final_start_value = TIM16_raw_start_value;
        TIM16_prescaler_adjust = DO_NOTHING;
        //Adjust_and_Set_TIM16_Prescaler(); //DO NOT COMMENT BACK IN
    #endif

    return 1;
}


