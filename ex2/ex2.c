#include <stdbool.h>
#include "efm32gg.h"
#include "gpio.h"
#include "sound.h"
#include "timer.h"
#include "dac.h"
#include "interrupt.h"

int main(void)
{
    *MSC_READCTRL   |= (1<<3);      // disable instruction cache
	gpio_config();
	dac_config();
	timer_config();
	nvic_config();

	
	sound_start(SOUND_3); 
	//timer_stop();
	//dac_stop();
	//timer_stop();
	sleep_deep();
	__asm ("wfi" : :);

    

	return 0;
}

