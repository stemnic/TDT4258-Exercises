#include <stdint.h>
#include <stdbool.h>
#include <efm32gg.h>
#include <nvic.h>
#include <dac.h>
#include <gpio.h>
#include <timer.h>

/*
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits. 
 */

#define   SAMPLE_PERIOD   0

void setup_nvic(void);

int main(void)
{
	gpio_init();
	dac_enable();
	setup_timer(SAMPLE_PERIOD);

	asm volatile ("cpsie if" : : : "memory");

	while (1);

	return 0;
}
