#ifndef INC_LED_H_
#define INC_LED_H_

//DEFINES
#define LED_BLINK_PERIOD 300
#define LED_COUNT_OFF_TIME 3

//INCLUDES
#include <stdint.h>

//CUSTOM TYPES
enum LED_States{

	LED_NONE, //startup only
	LED_OFF,
	LED_ON,
	LED_ONE_BLINK,
	LED_TWO_BLINK,
	LED_THREE_BLINK,
	LED_FOUR_BLINK,
	LED_CONFIRM,
};

//VARIABLES
volatile extern uint8_t LED_counter;
volatile extern uint32_t led_blink_period;

//FUNCTION DECLARATIONS


#endif /* INC_LED_H_ */
