#include <stdint.h>
#include <stdbool.h>
#include <efm32gg.h>
#include <types.h>
#include <nvic.h>

#define IRQ_TIMER1 5

void setup_timer(uint16_t period)
{
	// Enable and start timer 1 with interrupt:
	*(CMU_HFCORECLKEN0) |= CMU2_HFPERCLKEN0_TIMER1; 	//Enable timer 1
	*(TIMER1_IEN) |= (1 << 4);							//Enable timer 1 interrupt
	*(TIMER1_CMD) |= (1 << 0);							//Start timer 1 (write 1 to bit 1 to stop)

	nvic_enable(IRQ_TIMER1);
	nvic_set_priority(IRQ_TIMER1, NVIC_PRI_2);
}

volatile u32 t = 0;
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*const u32 coin_size = sizeof(sound_effect_coin) / sizeof(u16);
	if (t < coin_size) {
		u16 dac_val = sound_effect_coin[t];
		*(DAC0_CH0DATA) = dac_val;		
		t += 1;
	} else {
		//disable timer
		t = 0;
	}*/

	// CLear pending interrupt:
	*TIMER1_IFC = 1;
}
