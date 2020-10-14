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
	unsigned int const length;
};



/* Variables used in the interrupt */
volatile const unsigned short *curr_sample;
volatile const unsigned short *last_sample;

void sound_start(enum sound_name s);
void sound_stop(void);

#endif