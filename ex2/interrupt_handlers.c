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

/*
 * Decribes a sound 
 */
struct sound {
	const unsigned short *data;
	unsigned int length;
};

/* Remember same order as the enum */
static struct sound sound_list[NUM_SOUNDS] = {
	{.data = sound_1, .length = sizeof(sound_1) / 2},
	{.data = sound_2, .length = sizeof(sound_2) / 2},
	{.data = sound_3, .length = sizeof(sound_3) / 2}
};

/* Extern functions since they want us to write bad C code */
extern void start_timer(void);
extern void stop_timer(void);
extern void led_set(unsigned char led, unsigned char state);
extern unsigned char get_button(unsigned char btn);
extern void dac_write(unsigned short l, unsigned short r);

/* Variables used in the interrupt */
volatile unsigned short *curr_sample;
volatile unsigned short *last_sample;

/* State is not playing */
extern void stop_timer(void);
extern void led_set(unsigned char led, unsigned char state);
volatile unsigned char state = 0;

/*
 * Stops the current song from playing
 */
static void stop_sound(void)
{
	/* Stop song */
	state = 0;

	/* Reset pointers */
	curr_sample = NULL;
	last_sample = NULL;

	stop_timer();

	/* Enter LP mode */
	//*SCR = 0b10110;
	//asm volatile ("wfi");
}

/*
 * Starts a sound and enables the state flag so that the interrupt will start 
 * playing it
 */
void start_sound(enum sound_name s)
{
	/* Get the sound pointer */
	struct sound *sound = &sound_list[s];

	/* Stop playing */
	state = 0;

	curr_sample = sound->data;
	last_sample = sound->data + sound->length;

	start_timer();

	/* Start playing */
	state = 1;
}

/*
 * Main timer interrupt handler. This processes the current sound if the state 
 * vaiable is set. If will loop trough the sound until the last sample is 
 * fetched. 
 */
void __attribute__((interrupt)) TIMER1_IRQHandler()
{
	*TIMER1_IFC = 0xFFFF;

	if (!state)
		return;

	if (curr_sample && (curr_sample < last_sample)) {
		const unsigned short sample = *curr_sample++;
		dac_write(sample, sample);
	}
	else {
		stop_sound();
	}
}

/*
 * Both even and odd interrupt handler calls this function. This is called if 
 * any of the eight buttons is either pressed or released
 */
static inline void gpio_handler(void)
{
	/* Clear interrupt flags */
	*GPIO_IFC = 0xFFFF;

	if (!get_button(1)) {
		/* Button 0 pressed */
		start_sound(SOUND_1);
	} else if (!get_button(2)) {
		/* Button 1 pressed */
		start_sound(SOUND_2);
	} else if (!get_button(3)) {
		/* Button 2 pressed */
		start_sound(SOUND_3);
	} else {
		/* Button 2 released */
		//stop_sound();
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
