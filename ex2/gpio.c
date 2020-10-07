#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void gpio_config(void)
{
	/*
	 * Buttons C[0..7 ]
	 * Leds    A[8..15]
	 */
	*CMU_HFPERCLKEN0 |= (1 << 13);
	
	/* High drive strength on leds */
	*GPIO_PA_CTRL = 2;

	/* Push-pull with drive strength on all leds */
	*GPIO_PA_MODEH = 0x55555555;
	
	/* Turn off all leds */
	*GPIO_PA_DOUT = 0xFF00;

	/* Pull up */
	*GPIO_PC_DOUT = 0xFF;

	/* Input pull on all buttons */
	*GPIO_PC_MODEL = 0x22222222;

	/* Button interrupts on falling and rising edge */
	*GPIO_EXTIFALL = 0xFF;
	*GPIO_EXTIRISE = 0xFF;
	*GPIO_EXTIPSELL = 0x22222222;

	/* Clear interrupt flag and enable interrupts */
	*GPIO_IFC = 0xFFFF;
	*GPIO_IEN = 0xFF;
}

void led_set(unsigned char led, unsigned char state)
{
	/* LEDs are active low */
	if (state) {
		*GPIO_PA_DOUTCLR = (1 << (led + 8));
	} else {
		*GPIO_PA_DOUTSET = (1 << (led + 8));
	}
}

unsigned char get_button(unsigned char btn)
{
	return (*GPIO_PC_DIN & (1 << (btn - 1))) ? 1 : 0;
}
