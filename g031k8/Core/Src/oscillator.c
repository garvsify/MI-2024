//INCLUDES
#include <oscillator.h>

//CONSTANTS
const uint16_t sine_wavetable[512]={512,518,524,530,537,543,549,555,562,568,574,580,587,593,599,605,611,617,624,630,636,642,648,654,660,666,672,678,684,690,696,701,707,713,719,725,730,736,741,747,753,758,764,769,774,780,785,790,796,801,806,811,816,821,826,831,836,841,846,850,855,860,864,869,873,878,882,886,890,895,899,903,907,911,915,919,922,926,930,933,937,940,944,947,950,953,957,960,963,966,968,971,974,977,979,982,984,986,989,991,993,995,997,999,1001,1003,1004,1006,1008,1009,1011,1012,1013,1014,1015,1017,1017,1018,1019,1020,1021,1021,1022,1022,1022,1023,1023,1023,1023,1023,1023,1023,1022,1022,1022,1021,1021,1020,1019,1018,1017,1017,1015,1014,1013,1012,1011,1009,1008,1006,1004,1003,1001,999,997,995,993,991,989,986,984,982,979,977,974,971,968,966,963,960,957,953,950,947,944,940,937,933,930,926,922,919,915,911,907,903,899,895,890,886,882,878,873,869,864,860,855,850,846,841,836,831,826,821,816,811,806,801,796,790,785,780,774,769,764,758,753,747,741,736,730,725,719,713,707,701,696,690,684,678,672,666,660,654,648,642,636,630,624,617,611,605,599,593,587,580,574,568,562,555,549,543,537,530,524,518,512,505,499,493,486,480,474,468,461,455,449,443,436,430,424,418,412,406,399,393,387,381,375,369,363,357,351,345,339,333,327,322,316,310,304,298,293,287,282,276,270,265,259,254,249,243,238,233,227,222,217,212,207,202,197,192,187,182,177,173,168,163,159,154,150,145,141,137,133,128,124,120,116,112,108,104,101,97,93,90,86,83,79,76,73,70,66,63,60,57,55,52,49,46,44,41,39,37,34,32,30,28,26,24,22,20,19,17,15,14,12,11,10,9,8,6,6,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,6,6,8,9,10,11,12,14,15,17,19,20,22,24,26,28,30,32,34,37,39,41,44,46,49,52,55,57,60,63,66,70,73,76,79,83,86,90,93,97,101,104,108,112,116,120,124,128,133,137,141,145,150,154,159,163,168,173,177,182,187,192,197,202,207,212,217,222,227,233,238,243,249,254,259,265,270,276,282,287,293,298,304,310,316,322,327,333,339,345,351,357,363,369,375,381,387,393,399,406,412,418,424,430,436,443,449,455,461,468,474,480,486,493,499,505};
const uint16_t tri_wavetable[512]={512,516,520,524,528,532,536,540,544,548,552,556,560,564,568,572,576,580,584,588,592,596,600,604,608,612,616,620,624,628,632,636,640,644,648,652,656,660,664,668,672,676,680,684,688,692,696,700,704,708,712,716,720,724,728,732,736,740,744,748,752,756,760,764,768,771,775,779,783,787,791,795,799,803,807,811,815,819,823,827,831,835,839,843,847,851,855,859,863,867,871,875,879,883,887,891,895,899,903,907,911,915,919,923,927,931,935,939,943,947,951,955,959,963,967,971,975,979,983,987,991,995,999,1003,1007,1011,1015,1019,1023,1019,1015,1011,1007,1003,999,995,991,987,983,979,975,971,967,963,959,955,951,947,943,939,935,931,927,923,919,915,911,907,903,899,895,891,887,883,879,875,871,867,863,859,855,851,847,843,839,835,831,827,823,819,815,811,807,803,799,795,791,787,783,779,775,771,767,763,759,755,751,747,743,739,735,731,727,723,719,715,711,707,703,699,695,691,687,683,679,675,671,667,663,659,655,651,647,643,639,635,631,627,623,619,615,611,607,603,599,595,591,587,583,579,575,571,567,563,559,555,551,547,543,539,535,531,527,523,519,515,512,508,504,500,496,492,488,484,480,476,472,468,464,460,456,452,448,444,440,436,432,428,424,420,416,412,408,404,400,396,392,388,384,380,376,372,368,364,360,356,352,348,344,340,336,332,328,324,320,316,312,308,304,300,296,292,288,284,280,276,272,268,264,260,256,252,248,244,240,236,232,228,224,220,216,212,208,204,200,196,192,188,184,180,176,172,168,164,160,156,152,148,144,140,136,132,128,124,120,116,112,108,104,100,96,92,88,84,80,76,72,68,64,60,56,52,48,44,40,36,32,28,24,20,16,12,8,4,0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,236,240,244,248,252,256,260,264,268,272,276,280,284,288,292,296,300,304,308,312,316,320,324,328,332,336,340,344,348,352,356,360,364,368,372,376,380,384,388,392,396,400,404,408,412,416,420,424,428,432,436,440,444,448,452,456,460,464,468,472,476,480,484,488,492,496,500,504,508};
const uint16_t TIM16_prescalers[6] = {2048, 1024, 512, 256, 128, 64}; //2048 is used only by base prescaler index == 1 during symmetry adjustment
const uint8_t num_ADC_conversions = sizeof(ADCResultsDMA) / sizeof(ADCResultsDMA[0]);
const struct All_Params_Structs all_params_structs = {.running_params = &params,
													  .pot_control_params = &params_pot_control,
													  .to_be_loaded_params = &params_to_be_loaded,
													  .working_params = &params_working};

//VARIABLE DEFINITIONS
volatile uint16_t ADCResultsDMA[5] = {0};
volatile enum Validate initial_ADC_conversion_complete = NO;
volatile enum Validate sync_complete = NO;

//STRUCT VARIABLES
struct Params params = {0};
struct Params params_pot_control = {0};
struct Params params_to_be_loaded = {0};
struct Params params_working = {0};
struct Delay_Line delay_line = {.duty_delay_line_storage_array = 0, //one index larger than the number of indexes (wave samples) to allow us to 'wrap' the array into a kind of circular buffer - this is reinitialised to mid-scale on runtime
								.duty_delay_line_start_offset = 1,  //initial value is 1st index - to give us space to fill index 0
								.duty_delay_line_finish_offset = FINAL_INDEX + 1, //initial value is 512th index, one larger than the index of the final sample
								.duty_delay_line_read_pointer_offset = 0}; //determined in running

//FUNCTION DEFINITIONS
uint8_t Start_PWM_Gen_Timer_Main_and_Secondary_Oscillators(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel_1, uint32_t PWM_TIM_channel_2)
{
	uint8_t ok_OR = 0;
	ok_OR |= HAL_TIM_Base_Start(TIM);
	ok_OR |= HAL_TIM_PWM_Start(TIM, PWM_TIM_channel_1); //start PWM
	ok_OR |= HAL_TIM_PWM_Start(TIM, PWM_TIM_channel_2); //start PWM

	if(ok_OR != HAL_OK){

		Error_Handler();
	}

	return ok_OR;
}

uint8_t Start_Freq_Gen_Timer(void)
{
	uint8_t ok = Start_OC_TIM(&htim16, TIM_CHANNEL_1); //start freq. gen.

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
    speed_control = speed_control >> SPEED_ADC_RESOLUTION;

    how_many_128 = (uint8_t)(speed_control >> 7); //divide by 128, i.e. return how many 128s go into the speed_control
    params_ptr->raw_start_value = (uint16_t)(speed_control - (uint16_t)(how_many_128 << 7)); //how_many_128*128
    params_ptr->raw_prescaler = SLOWEST_SPEED_PRESCALER >> how_many_128;

    return 1;
}

uint8_t Set_Oscillator_Values(struct Params* params_ptr){

	////////////////////////////////////////////////////////
	//SET THE CURRENT(prev) VALUES FOR THE MAIN OSCILLATOR//
	////////////////////////////////////////////////////////
	__HAL_TIM_SET_AUTORELOAD(&htim16, params_ptr->final_ARR);
	__HAL_TIM_SET_PRESCALER(&htim16, params_ptr->final_prescaler_minus_one); //have to take one off the divisor

	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, params_ptr->prev_duty); //updates the CCR register of TIM14, which sets duty, i.e. the ON time relative to the total period which is set by the ARR.

	/////////////////////////////////////////////////////////////
	//SET THE CURRENT(prev) VALUES FOR THE SECONDARY OSCILLATOR//
	/////////////////////////////////////////////////////////////
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, params_ptr->duty_delayed); //updates the CCR register of TIM14, which sets duty, i.e. the ON time relative to the total period which is set by the ARR.

	return 1;
}

uint8_t Calculate_Next_Main_Oscillator_Values(const struct All_Params_Structs *all_params_structs_ptr , enum Next_Values_Processing_Mode mode){

	struct Params* params_ptr = NULL;

	if(mode == REGULAR_MODE){

		params_ptr = all_params_structs_ptr->running_params;

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

		Copy_Params_Structs(all_params_structs_ptr->running_params, all_params_structs_ptr->to_be_loaded_params);

		params_ptr = all_params_structs_ptr->to_be_loaded_params;

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
	else if(mode == STARTUP_MODE){

		params_ptr = all_params_structs_ptr->pot_control_params;

		if(params_ptr->index == FIRST_QUADRANT_START_INDEX){
			params_ptr->quadrant = FIRST_QUADRANT;
			params_ptr->halfcycle = FIRST_HALFCYCLE;
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
			params_ptr->duty = PWM_DUTY_VALUE_MAX - (multiply_product >> DEPTH_ADC_RESOLUTION);
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
	uint16_t ADC_result = ADCResultsDMA_ptr[WAVESHAPE_ADC_RESULT_INDEX] >> 5; //set ADC_Result to waveshape index value, truncate to 7-bit

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
	params_ptr->speed = ADCResultsDMA_ptr[SPEED_ADC_RESULT_INDEX] >> 5; //truncate to 7-bit
	params_ptr->speed <<= 3; //convert to 10-bit

	//GET DEPTH
	#if DEPTH_ON_OR_OFF == ON

		params_ptr->depth = ADCResultsDMA_ptr[DEPTH_ADC_RESULT_INDEX] >> 5; //truncate to 7-bit

	#endif

	//GET SYMMETRY
	#if SYMMETRY_ON_OR_OFF == ON

		params_ptr->symmetry = ADCResultsDMA_ptr[SYMMETRY_ADC_RESULT_INDEX] >> 5; //truncate to 7-bit
		params_ptr->symmetry <<= 1; //convert to 8-bit

	#endif

	//GET DELAY LINE READ POINTER OFFSET
	delay_line_ptr->duty_delay_line_read_pointer_offset = ADCResultsDMA_ptr[DUTY_DELAY_LINE_READ_POINTER_OFFSET_ADC_RESULT_INDEX] >> 5; //truncate to 7-bit
	delay_line_ptr->duty_delay_line_read_pointer_offset <<= 2; //convert to 9-bit

	return 1;
}

uint8_t Copy_Params_Structs(struct Params* src_ptr, struct Params* dst_ptr){

	*dst_ptr = *src_ptr;

	return 1;
}



