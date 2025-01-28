#ifndef INC_SYMMETRY_ADJUSTMENT_TYPES_H_
#define INC_SYMMETRY_ADJUSTMENT_TYPES_H_

//CUSTOM TYPES
enum TIM16_final_start_value_Oscillation_Mode{
	DO_NOT_OSCILLATE,
	OSCILLATE_UPWARDS,
	OSCILLATE_DOWNWARDS
};

enum Symmetry_Status{
	CW,
	CCW
};

enum Symmetry_Type{
	SHORTEN,
	LENGTHEN
};

enum Adjust_Prescaler_Action{
	DO_NOTHING,
	MULTIPLY_BY_TWO,
};

#endif /* INC_SYMMETRY_ADJUSTMENT_TYPES_H_ */
