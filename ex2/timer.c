#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * function to setup the timer 
 */
void setupTimer(uint16_t period)
{
	// Enable and start timer 1 with interrupt:
	*(CMU_HFCORECLKEN0) |= CMU2_HFPERCLKEN0_TIMER1; //Enable timer 1
	*(TIMER1_IEN) |= (1 << 4);						//Enable timer 1 interrupt
	*(TIMER1_CTRL) |= (10 << 24);					// 1024 times prescaler

	*(TIMER1_CMD) = (1 << 0); //Start timer 1 (write 1 to bit 1 to stop)
	*(TIMER1_TOP) = 4000;	  // 4 MHz clock -> 1024 prescaler -> count to 4000 -> interrupt every second-ish
	nvic_enable(IRQ_TIMER1);
	nvic_set_priority(IRQ_TIMER1, NVIC_PRI_2);
	/*
	 * TODO enable and set up the timer
	 * 
	 * 1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0 2.
	 * Write the period to register TIMER1_TOP 3. Enable timer interrupt
	 * generation by writing 1 to TIMER1_IEN 4. Start the timer by writing 
	 * 1 to TIMER1_CMD
	 * 
	 * This will cause a timer interrupt to be generated every (period)
	 * cycles. Remember to configure the NVIC as well, otherwise the
	 * interrupt handler will not be invoked. 
	 */
}
