#include <stdbool.h>
#include "efm32gg.h"
#include "gpio.h"
#include "sound.h"
#include "timer.h"
#include "dac.h"
#ifdef INTERRUPT
	#include "interrupt.h"
#endif

int main(void)
{
    *MSC_READCTRL   |= (1<<3);      // disable instruction cache
	gpio_config();
	dac_config();
	timer_config();

	#ifdef INTERRUPT
		nvic_config();
		sound_start(SOUND_3);
		//timer_stop();
		//dac_stop();
		//timer_stop();
		sleep_deep();
		__asm ("wfi" : :);
	#else
		timer_start();
		timer_set_top(0xFFFF);
		sound_start(SOUND_3);
		while (1) {
			//this while loop polls buttons and timer1

			const uint32_t top = 400;
			if (*TIMER1_CNT > top) {
				//reset timer counter
				*TIMER1_CNT = 0;
				if (curr_sample && (curr_sample < last_sample)) 
				{
					//play current sample and increment
					const unsigned short sample = *curr_sample++;
					dac_write(sample, sample);
				}
			}

			if (!get_button(1)) {
				/* Button 1 pressed */
				sound_start(SOUND_1);
			} else if (!get_button(2)) {
				/* Button 2 pressed */
				sound_start(SOUND_2);
			} else if (!get_button(3)) {
				/* Button 3 pressed */
				sound_start(SOUND_3);
			} 
		}
	#endif
    

	return 0;
}

