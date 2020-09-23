#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * function to setup the timer
 */
void setup_timer(uint16_t period)
{
	// Enable and start timer 1 with interrupt:
	*(CMU_HFCORECLKEN0) |= CMU2_HFPERCLKEN0_TIMER1; 
	*(TIMER1_IEN) |= (1 << 4);
	*(TIMER1_CMD) |= (1 << 0);	
}
