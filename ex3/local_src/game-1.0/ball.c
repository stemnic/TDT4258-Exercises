#include "ball.h"
#include <stdbool.h>

void ball_create(struct Ball* ball, int startpos_x, int startpos_y) {
    ball->pos_x = startpos_x;
    ball->pos_y = startpos_y;
    ball->vel_x = 1;
    ball->vel_y = 1;
}

bool ball_detect_collision(struct Ball* ball, struct Paddle *paddle_left, struct Paddle *paddle_right) {
    if (ball->pos_x == 0) {     //right victory
        ball->pos_x = GAME_WIDTH / 2;
        return true;
    }
    if (ball->pos_x > GAME_WIDTH - 1) {    //left victory
        ball->pos_x = GAME_WIDTH / 2;
        return true;
    }

    if (ball->pos_y == 0 && ball->vel_y == -1)      //bounce against upper wall
        ball->vel_y = 1;
    if (ball->pos_y == GAME_HEIGHT - 1 && ball->vel_y == 1)     //bounce against lower wall
        ball->vel_y = -1;

    if (ball->pos_x == paddle_left->pos_x + 1 && ball->vel_x == -1) {
        //check for collision with left paddle
        if (ball->pos_y <= paddle_left->pos_y + PADDLE_WIDTH && ball->pos_y >= paddle_left->pos_y)
            ball->vel_x = 1;
    }

    if (ball->pos_x == paddle_right->pos_x - 1 && ball->vel_x == 1) {
        //check for collision with right paddle
        if (ball->pos_y <= paddle_right->pos_y + PADDLE_WIDTH && ball->pos_y >= paddle_right->pos_y)
            ball->vel_x = -1;
    }

    return false;  
}

void ball_move(struct Ball* ball) {
    ball->pos_x += ball->vel_x;
    ball->pos_y += ball->vel_y;
}