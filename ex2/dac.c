#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "dac.h"

void dac_config(void)
{
	/*
	 * 1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0
	 * 2. Prescale DAC clock by writing 0x50010 to DAC0_CTRL
	 * 3. Enable left and right audio channels by writing 1 to DAC0_CH0CTRL and
	 *    DAC0_CH1CTRL
	 * 4. Write a continuous stream of samples to the DAC data registers, 
	 *    DAC0_CH0DATA and DAC0_CH1DATA, for example from a timer interrupt 
	 */
	*CMU_HFPERCLKEN0 |= (1 << 17);
	*DAC0_CTRL = 0x50010;
	*DAC0_CH0CTRL = 1;
	*DAC0_CH1CTRL = 1;
}

void dac_write(unsigned short l, unsigned short r)
{
	*DAC0_CH0DATA = (l & 0xFFF);
	*DAC0_CH1DATA = (r & 0xFFF);
}

void dac_start(void)
{
	*CMU_HFPERCLKEN0 |= (1 << 17);
	*DAC0_CTRL = 0x50010;
	*DAC0_CH0CTRL = 1;
	*DAC0_CH1CTRL = 1;
}

void dac_stop(void)
{
	*DAC0_CH0CTRL = 0;
	*DAC0_CH1CTRL = 0;
	*CMU_HFPERCLKEN0 &= ~(1 << 17);
}