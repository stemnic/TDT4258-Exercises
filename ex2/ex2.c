#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

void setupTimer(void);
void setupDAC(void);
void setupNVIC(void);
void setupGPIO(void);

int main(void)
{
	setupGPIO();
	setupDAC();
	setupTimer();
	setupNVIC();

	while (1);
	return 0;
}

void setupNVIC()
{
	/*
	 * TODO use the NVIC ISERx registers to enable handling of
	 * interrupt(s) remember two things are necessary for interrupt
	 * handling: - the peripheral must generate an interrupt signal - the
	 * NVIC must be configured to make the CPU handle the signal You will
	 * need TIMER1, GPIO odd and GPIO even interrupt handling for this
	 * assignment. 
	 */
	*ISER0 = (1 << 1) | (1 << 11) | (1 << 12);
}
