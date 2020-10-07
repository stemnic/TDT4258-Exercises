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

	while (1);
	return 0;
}

