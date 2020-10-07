#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "efm32gg.h"
#include "sound.h"
#include "gpio.h"
#include "timer.h"
#include "dac.h"

void nvic_config()
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


/*
 * Main timer interrupt handler. This processes the current sound if the state 
 * vaiable is set. If will loop trough the sound until the last sample is 
 * fetched. 
 */
void __attribute__((interrupt)) TIMER1_IRQHandler()
{
	wake_up();
	*TIMER1_IFC = 0xFFFF;

	if (curr_sample && (curr_sample < last_sample)) {
		//play current sample and increment
		const unsigned short sample = *curr_sample++;
		dac_write(sample, sample);
	}
	else {
		//sound is complete, return to deep sleep
		sound_stop();
		timer_stop();
		sleep_deep();
	}
}


/*
 * Both even and odd interrupt handler calls this function. This is called if 
 * any of the eight buttons is either pressed or released
 */
static inline void gpio_handler(void)
{
	wake_up();
	/* Clear interrupt flags */
	*GPIO_IFC = 0xFFFF;
	
	if (!get_button(1)) {
		/* Button 0 pressed */
		sound_start(SOUND_1);
		led_set(1, 1);

	} else if (!get_button(2)) {
		/* Button 1 pressed */
		sound_start(SOUND_2);
		led_set(2, 1);
	} else if (!get_button(3)) {
		/* Button 2 pressed */
		sound_start(SOUND_3);
	} 
}

void __attribute__((interrupt)) GPIO_EVEN_IRQHandler()
{
	gpio_handler();
}

void __attribute__((interrupt)) GPIO_ODD_IRQHandler()
{
	gpio_handler();
}
