#include "display.h"

struct fb_copyarea frame;
uint16_t *framebuffer;
int fbfd = 0;

Color color(uint8_t r, uint8_t g, uint8_t b) {
	Color col;
	if(r >= 0 && r < 32 && g >= 0 && g < 64 && b >= 0 && b < 32){
		col.R = r;
		col.G = g;
		col.B = b;
	} else {
		col.R = 0;
		col.G = 0;
		col.B = 0;
		printf("Invalid color value supplied: (R: %d, G: %d, B: %d), max (31, 63, 31)", r, g, b);
	}
	return col;
}

void init_display(){
    // Init display driver
	frame.dx = 0;
	frame.dy = 0;
	frame.width = SCREEN_WIDTH;
	frame.height = SCREEN_HEIGHT;
	fbfd = open("/dev/fb0", O_RDWR);
	if(fbfd == -1) {
		// Failed to open file descriptor
		printf("Error: unable to open framebuffer, exiting...\n)");
		exit(EXIT_FAILURE);
	}
	int screensize = SCREEN_WIDTH * SCREEN_HEIGHT * SCR_BPP; 	
	framebuffer = (uint16_t*) mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if(framebuffer == MAP_FAILED) { // Error, return
		printf("Error: unable to map framebuffer, exiting...\n");
		close(fbfd);
		exit(EXIT_FAILURE);
	}
}

void set_screen_black(){
	memset(framebuffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * SCR_BPP); // Fill screen with black
}


void flush_display_buffer() {
    ioctl(fbfd, 0x4680, &frame);
}

void draw_pixel(uint16_t x, uint16_t y, Color color) {
	if(x < SCREEN_WIDTH && x >= 0 && y < SCREEN_HEIGHT && y >= 0) { // validate pixel position
		framebuffer[ x + y*SCREEN_WIDTH ] = color.toint;
	} else {
		printf("Invalid pixel position supplied: (%d,%d), max %d x %d", x, y, (SCREEN_WIDTH-1), (SCREEN_HEIGHT-1));
	}
}

void draw_paddle(struct Paddle* paddle) {
	for (int i = 0; i < 2 * PADDLE_WIDTH; ++i) {
		draw_pixel(2*paddle->pos_x, 2*paddle->pos_y + i, color(31, 0, 0));
		draw_pixel(2*paddle->pos_x + 1, 2*paddle->pos_y + i, color(31, 0, 0));
	}
}
void draw_ball(struct Ball* ball) {
	draw_pixel(2*ball->pos_x, 2*ball->pos_y, color(0, 63, 0));
	draw_pixel(2*ball->pos_x + 1, 2*ball->pos_y, color(0, 63, 0));
	draw_pixel(2*ball->pos_x + 1, 2*ball->pos_y + 1, color(0, 63, 0));
	draw_pixel(2*ball->pos_x, 2*ball->pos_y + 1, color(0, 63, 0));
}// her kan den plutselig dette utafor skjermen.... BOOM! Kan evt snu når det er en pixel igjen

