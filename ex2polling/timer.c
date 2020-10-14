#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "dac.h"

#include "efm32gg.h"

void timer_config(void)
{
	/*
	 * 1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0
	 * 2. Write the period to register TIMER1_TOP
	 * 3. Enable timer interrupt generation by writing 1 to TIMER1_IEN
	 * 4. Start the timer by writing 1 to TIMER1_CMD
	 */
	 *CMU_HFPERCLKEN0 |= (1 << 6);
	 

	 /*
	  * Calculate the prescaler
	  * period_us = 1.000.000 / sample_rate
	  * period_us = 1.000.000 / 44.000 = 22.7 us
	  * 
	  * The source clock is 14 MHz
	  * period_timer_us = 1.000.000 * prescaler / 14.000.000
	  * period_timer_us = 1.000.000 * 8         / 14.000.000 = 0.57 us
	  * 
	  * ticks = period_us / period_timer_us
	  * ticks = 22.7 us / 0.57 us = 40 ticks
	  * 
	  * With a prescaler of 8 the period should be 80 in order to produce a 
	  * tick rate of 22 kHz.
	  */
	 
	 /* Set the prescaler to 8 */
	 *TIMER1_CTRL &= ~(0xF << 24);
	 *TIMER1_CTRL &= ~(0b00 << 0); // |= (3 << 24);
}

void timer_start(void)
{
    *CMU_HFPERCLKEN0 |= (1 << 6);
	*TIMER1_CMD = 1;
}

void timer_stop(void)
{
    *TIMER1_CMD = (1 << 1);
    *CMU_HFPERCLKEN0 &= ~(1 << 6);	
}

void timer_set_top(uint32_t top)
{
	*TIMER1_TOP = top;
}
