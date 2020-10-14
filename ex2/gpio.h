#ifndef GPIO_H
#define GPIO_H

void gpio_config(void);
void led_set(unsigned char led, unsigned char state);
unsigned char get_button(unsigned char btn);

#endif