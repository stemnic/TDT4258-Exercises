#include <stdint.h>
#include <stdbool.h>
#include <types.h>
#include <efm32gg.h>

/* 
	* 1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0 2.
	* Prescale DAC clock by writing 0x50010 to DAC0_CTRL 3. Enable left
	* and right audio channels by writing 1 to DAC0_CH0CTRL and
	* DAC0_CH1CTRL 4. Write a continuous stream of samples to the DAC
	* data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a
	* timer interrupt 
*/
void dac_enable()
{
	*CMU_HFPERCLKEN0 |= (1<<17);	// enable DAC clock
	*DAC0_CTRL = 0x50010;			// set DAC clock with prescaler 32 -> 437.5KHz when main clock on 14MHz
	*DAC0_CH0CTRL = 1; 				// enable channel 0
	*DAC0_CH1CTRL = 1;				// enable channel 1
}

void dac_disable()
{
	*DAC0_CH0CTRL = 0;				// disable ch0
	*DAC0_CH1CTRL = 0; 				// disable ch1
	//*DAC0_CTRL &= ~(0x50010);		// disable clock prescaler ??
	*CMU_HFPERCLKEN0 &= ~(1<<17); 	// disable DAC clock
}

void dac_write(u16 data)
{
	*DAC0_CH0DATA = data | 0xFFF; 	// write next data to ch0 on the DAC
	*DAC0_CH1DATA = data | 0xFFF; 	// write next data to ch1 in the DAC
}