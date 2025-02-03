#ifndef INC_CLK_IN_H_
#define INC_CLK_IN_H_

//MACROS
#define TIM17_OVERFLOW_LENGTH 10 /// 10-1 is about 5ms overflow period with /4/512/64 division-prescaler

//INCLUDES
#include <stdint.h>

//CUSTOM TYPES
enum CLK_IN_State{

	LOW,
	HIGH
};


#endif /* INC_CLK_IN_H_ */
