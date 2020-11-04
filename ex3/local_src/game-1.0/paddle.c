#include "paddle.h"


void paddle_create(struct Paddle* paddle, int start_pos_x) {
    paddle->pos_x=start_pos_x;
    paddle->pos_y=GAME_HEIGHT / 2;
}


void paddle_move_up(struct Paddle* paddle) {
    if (paddle->pos_y >= 2)
        paddle->pos_y -= 2;
}

void paddle_move_down(struct Paddle* paddle) {
    if (paddle->pos_y + PADDLE_WIDTH <= GAME_HEIGHT - 3)
        paddle->pos_y += 2;
}