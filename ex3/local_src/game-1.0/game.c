#include "game.h"
#include "paddle.h"
#include "ball.h"
#include "display.h"

static FILE* dev_gamepad;
static struct Paddle paddle_left;
static struct Paddle paddle_right;
static struct Ball ball;

int main(int argc, char *argv[])
{
    paddle_create(&paddle_left, 10);
    paddle_create(&paddle_right, GAME_WIDTH - 10);
    ball_create(&ball, GAME_WIDTH / 2, GAME_HEIGHT / 2);
    init_display();
	printf("Hello World, I'm game!\n");
    set_screen_black();
    draw_pixel(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 , color(30,30,30));
    gamepad_init();
    while (true) {
        //main game loop
        set_screen_black();
        ball_detect_collision(&ball, &paddle_left, &paddle_right);
        ball_move(&ball);
        draw_ball(&ball);
        draw_paddle(&paddle_left);
        draw_paddle(&paddle_right);
        flush_display_buffer();
    }

	exit(EXIT_SUCCESS);
}

int gamepad_init()
{
    dev_gamepad = fopen("/dev/gamepad", "rb");
    if (!dev_gamepad) {
        printf("Unable to open driver dev_gamepad, maybe you didn't load the module?\n");
        return EXIT_FAILURE;
    }
    if (signal(SIGIO, &sigio_handler) == SIG_ERR) {
        printf("An error occurred while register a signal handler.\n");
        return EXIT_FAILURE;
    }
    if (fcntl(fileno(dev_gamepad), F_SETOWN, getpid()) == -1) {
        printf("Error setting pid as owner.\n");
        return EXIT_FAILURE;
    }
    long oflags = fcntl(fileno(dev_gamepad), F_GETFL);
    if (fcntl(fileno(dev_gamepad), F_SETFL, oflags | FASYNC) == -1) {
        printf("Error setting FASYNC flag.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void sigio_handler(int signo)
{
	const int UP_PLAYER_1 = 3;
    const int DOWN_PLAYER_1 = 1;
    const int UP_PLAYER_2 = 7;
    const int DOWN_PLAYER_2 = 5;
    int input = fgetc(dev_gamepad);
    if (input & (1 << UP_PLAYER_1))
        paddle_move_up(&paddle_left);
    if (input & (1 << DOWN_PLAYER_1))
        paddle_move_down(&paddle_left);
    if (input & (1 << UP_PLAYER_2))
        paddle_move_up(&paddle_right);
    if (input & (1 << DOWN_PLAYER_2))
        paddle_move_down(&paddle_right);
}