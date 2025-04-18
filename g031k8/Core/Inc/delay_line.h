#ifndef INC_DELAY_LINE_H_
#define INC_DELAY_LINE_H_

//INCLUDES
#include <stdint.h>

//CUSTOM TYPES
struct Delay_Line{

	volatile uint16_t duty_delay_line_storage_array[513]; //one index larger than the number of indexes (wave samples) to allow us to 'wrap' the array into a kind of circular buffer
	volatile uint16_t duty_delay_line_start_offset; //initial value is 1st index
	volatile uint16_t duty_delay_line_finish_offset; //initial value is 512th index (513th value)
	volatile uint16_t duty_delay_line_read_pointer_offset;
};

#endif /* INC_DELAY_LINE_H_ */
