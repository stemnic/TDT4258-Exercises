#ifndef DAC_H
#define DAC_H

void dac_config(void);
void dac_write(unsigned short l, unsigned short r);
void dac_start(void);
void dac_stop(void);
#endif