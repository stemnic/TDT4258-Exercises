#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

void timer_config(void);
void timer_set_top(uint32_t top);
void timer_start(void);
void timer_stop(void);

void sleep_deep();
void wake_up();

#endif