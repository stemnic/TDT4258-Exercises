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
 * 
 * Reference manual
 * https://www.silabs.com/documents/public/reference-manuals/EFM32GG-RM.pdf
 */

#define SAMPLE_PERIOD 0

void setup_nvic(void);

int main(void)
{
	gpio_init();
	dac_enable();
	setup_timer(SAMPLE_PERIOD);

	asm volatile("cpsie if" : : : "memory");

	u8 led_is_on = 1;
	while (1)
	{
		for (volatile u32 i = 0; i < 1000000; i++) {
			asm volatile ("nop");
		}
		gpio_set_led(1, led_is_on);
		led_is_on = led_is_on ^ 1;
	}

	return 0;
}
