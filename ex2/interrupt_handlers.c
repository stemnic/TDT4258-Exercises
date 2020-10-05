#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "efm32gg.h"
#include "sound.h"

#define NUM_SOUNDS 3

enum sound_name {
	SOUND_1,
	SOUND_2,
	SOUND_3
};

struct sound {
	unsigned short* data;
	unsigned int length;
};

/* Remember same order as the enum */
static struct sound sound_list[NUM_SOUNDS] = {
	{ .data = sound_1, .length = sizeof(sound_1) / 2 },
	{ .data = sound_2, .length = sizeof(sound_2) / 2 },
	{ .data = sound_3, .length = sizeof(sound_3) / 2 }
};

/* Extern functions since they want us to write bad C code */
extern void start_timer(void);
extern void stop_timer(void);
extern void led_set(unsigned char led, unsigned char state);
extern unsigned char get_button(unsigned char btn);
extern void dac_write(unsigned short l, unsigned short r);

/* Variables used in the interrupt */
volatile unsigned short* curr_sample;
volatile unsigned short* last_sample;

/* State is not playing */
volatile unsigned char state = 0;

static void stop_sound(void)
{
	/* Stop song */
	state = 0;

	/* Reset pointers */
	curr_sample = NULL;
	last_sample = NULL;

	stop_timer();

	/* Optional enter LP mode */
}

static void start_sound(enum sound_name s)
{
	/* Get the sound pointer */
	struct sound* sound = &sound_list[s];

	/* Stop playing */
	state = 0;

	curr_sample = sound->data;
	last_sample = sound->data + sound->length;

	start_timer();

	/* Start playing */
	state = 1;
}

void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	*TIMER1_IFC = 1;
	
	if (!state) return;

	if (curr_sample && (curr_sample < last_sample)) {
		unsigned short sample = *curr_sample++;
		dac_write(sample, sample);
	}
}

static inline void gpio_handler(void)
{
	/* Clear interrupt flags */
	*GPIO_IFC = *GPIO_IF;

	if (!get_button(0)) {
		/* Button 0 pressed */
		start_sound(SOUND_1);
	}

	if (get_button(0)) {
		/* Button 0 released */
	}
}

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	gpio_handler();
}

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	gpio_handler();
}
