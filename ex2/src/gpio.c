#include <stdint.h>
#include <stdbool.h>
#include <types.h>
#include <efm32gg.h>

static inline u8 get_button_val(u8 button);
static inline void set_led(u8 led, u8 val);

/*
 * function to set up GPIO mode and interrupts
 */
void gpio_init(void)
{
	/*
	 * TODO set input and output pins for the joystick 
	 */

	/* enable GPIO clock */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;

	/* set high drive strength */
	*GPIO_PA_CTRL = 2;

	/* set pins A8-15 as output */
	*GPIO_PA_MODEH = 0x55555555;

	/* turn on LEDs D4-D8 (LEDs are active low) */
	*GPIO_PA_DOUT = 0x0700;

	/* Set input and output pins for the joystick  GPIOC 0..7 */

	/* Input mode pull-mode on all pins */
	*GPIO_PC_MODEL = 0;
	for (u32 i = 0; i < 8; i++) {
		*GPIO_PC_MODEL |= (2 << (i * 4));
	}

	/* Pull-ups on all pins */
	*GPIO_PC_DOUT = 0xFF;

	while (1) {
		u32 reg = *GPIO_PC_DIN;
		*GPIO_PA_DOUT = reg << 8;
	}

}

static inline void set_led(u8 led, u8 val)
{
	if (val) {
		*GPIO_PA_DOUTSET = (1 << (led + 8));
	} else {
		*GPIO_PA_DOUTCLR = (1 << (led + 8));
	}
}

static inline u8 get_button_val(u8 button)
{
	return (*GPIO_PC_DIN & (1 << button)) ? 1 : 0;
}

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	
}

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{

}
