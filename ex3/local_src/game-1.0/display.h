#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "ball.h"
#include "paddle.h"

#ifndef DISPLAY_H
#define DISPLAY_H

// Define screen dimensions as constants
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCR_BPP 2  //16 bits per pixel (in bytes)

typedef union {
	struct {
		uint16_t B : 5;
		uint16_t G : 6;
		uint16_t R : 5;
	};
	uint16_t toint;
} Color;

Color color(uint8_t r, uint8_t g, uint8_t b);

void init_display();
void flush_display_buffer();
void set_screen_black();
void draw_pixel(uint16_t x, uint16_t y, Color color);
void draw_paddle(struct Paddle* paddle);
void draw_ball(struct Ball* ball);

#endif