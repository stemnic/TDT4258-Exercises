#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * TIMER1 interrupt handler 
 */
volatile uint32_t tick = 0;
volatile uint32_t data = 255;
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	 * TODO feed new samples to the DAC remember to clear the pending
	 * interrupt by writing 1 to TIMER1_IFC 
	 */
	*TIMER1_IFC = 1;
	tick++;


	if (tick >= 10) {
		tick = 0;
		*DAC0_CH0DATA = data;
		*DAC0_CH1DATA = data;

		data = (data) ? 0 : 255;
	}
}

/*
 * GPIO even pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
}

/*
 * GPIO odd pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
}
}

/*
 * GPIO odd pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
}
