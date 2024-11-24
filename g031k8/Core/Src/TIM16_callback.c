//INCLUDES
#include "TIM16_callback.h"

//VARIABLES

//FUNCTIONS
void TIM16_callback(TIM_HandleTypeDef *htim){

	//TMR0H = (uint8_t)final_TMR0; //this line must go here, or at least very near the beginning!

	//LATC5 = 1; //start ISR length measurement
	//TMR0IF = 0; //clear TMR0 interrupt flag

	if(current_waveshape == TRIANGLE_MODE){
		duty = tri_table_one_quadrant[current_one_quadrant_index];
	}
	else if(current_waveshape == SINE_MODE){
		duty = sine_table_one_quadrant[current_one_quadrant_index];
	}
	else if(current_waveshape == SQUARE_MODE){
		duty = 1023;
	}
	if(current_one_quadrant_index == MAX_QUADRANT_INDEX){
		current_quadrant = SECOND_QUADRANT;
	}
	else if(current_one_quadrant_index == MIN_QUADRANT_INDEX){
		current_quadrant = FIRST_QUADRANT;
		if(current_halfcycle == FIRST_HALFCYCLE){
			current_halfcycle = SECOND_HALFCYCLE;
		}
		else{
			current_halfcycle = FIRST_HALFCYCLE;
		}
	}
	if(current_quadrant == FIRST_QUADRANT){
		current_one_quadrant_index++;
	}
	else{
		current_one_quadrant_index--;
	}
	if(current_halfcycle == SECOND_HALFCYCLE){
		duty = 1023 - duty;
	}

#if DEPTH_ON_OR_OFF == 1
	//Apply Depth
	if(current_depth == 255){
		duty = 1023 - duty;
	}
	else if(current_depth != 0){
		//duty = 1023 - duty*(current_depth >> 8);
		multiply_duty_by_current_depth_and_divide_by_256();
	}
	else{
		duty = 1023; //if depth is 0, just output 1023
	}
#endif

	//Write Duty
	//CCP1_LoadDutyValue(duty); //-to be sorted

	//Finish Up
	//LATC5 = 0; //finish ISR length measurement //-to be sorted
}

/*uint8_t multiply_duty_by_current_depth_and_divide_by_256(void){ //-to be sorted

	volatile uint8_t res0 = 0;
	volatile uint8_t res1 = 0;
	volatile uint8_t res2 = 0;
	volatile uint8_t res3 = 0;
	volatile uint8_t dutyL = 0;
	volatile uint8_t dutyH = 0;
	volatile uint8_t current_depthL = 0;
	volatile uint16_t result_of_low_by_low = 0;
	volatile uint32_t result_of_low_by_high = 0;
	volatile uint32_t multiply_product = 0;

    dutyL = (uint8_t)duty;
    dutyH = duty >> 8;
    current_depthL = (uint8_t)current_depth;

    asm("MOVF _current_depthL, W");
    asm("MULWF _dutyL");
    asm("MOVFF PRODH, _res1");
    asm("MOVFF PRODL, _res0");
    result_of_low_by_low = (uint16_t)((uint16_t)(res1 << 8) | res0);

    asm("MOVF _current_depthL, W");
    asm("MULWF _dutyH");
    asm("MOVFF PRODH, _res3");
    asm("MOVFF PRODL, _res2");
    result_of_low_by_high = (uint32_t)(((uint32_t)(res3 << 8) | (uint32_t)res2) << 8);

    multiply_product = result_of_low_by_high + result_of_low_by_low;
    duty = 1023 - (uint16_t)(multiply_product >> 8);

    return 1;
}*/
