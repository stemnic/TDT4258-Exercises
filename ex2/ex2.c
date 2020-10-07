#include <stdbool.h>
#include "efm32gg.h"
#include "gpio.h"
#include "sound.h"
#include "timer.h"
#include "dac.h"
#include "interrupt.h"

int main(void)
{
	gpio_config();
	dac_config();
	timer_config();
	nvic_config();

	
	sound_start(SOUND_3); 
	//timer_stop();
	//dac_stop();
	//timer_stop();
	sleep_deep();

    //*EMU_MEMCTRL    = 7;            // disable ram blocks 1-3
    //*MSC_READCTRL   |= (1<<3);      // disable instruction cache

	while (1);
	return 0;
}

