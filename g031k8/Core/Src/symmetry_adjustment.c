#include "symmetry_adjustment.h"


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

			//uint16_t two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC = (two_fifty_six_minus_TIM16_raw_start_value * pot_rotation_corrected);
			//COMMENT LINE BELOW IN AND LINE ABOVE OUT TO EXPERIMENT WITH MORE EXTREME SYMMETRY ADJUSTMENT
			uint16_t two_fifty_six_minus_TIM16_raw_start_value_multiplied_by_PRC = ((two_fifty_six_minus_TIM16_raw_start_value * pot_rotation_corrected) * 11) >> 3; //x(11/8)

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

    params_ptr->final_period = 256 - params_ptr->final_start_value;
    params_ptr->final_ARR = params_ptr->final_period - 1;
    params_ptr->final_prescaler_minus_one = params_ptr->final_prescaler - 1;

    return 1;
}
