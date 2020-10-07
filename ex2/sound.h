#ifndef SOUND_H
#define SOUND_H

#define NUM_SOUNDS 3

/*
 *  The data for the three sounds are stored in arrays directly exported from wav files
 */
const unsigned short sound_1[2004];
const unsigned short sound_2[1162];
const unsigned short sound_3[2049];

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

/* Variables used in the interrupt */
volatile unsigned short *curr_sample;
volatile unsigned short *last_sample;
/* State is not playing */
volatile unsigned char state;

void sound_start(enum sound_name s);
void sound_stop(void);


#endif