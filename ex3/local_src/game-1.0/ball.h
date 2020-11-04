#include "paddle.h"
#include <stdbool.h>

#ifndef BALL_H
#define BALL_H

struct Ball{
    //ball used in pong, has diagonal velocity, collides with walls and paddles
    int pos_x, pos_y;
    int vel_x, vel_y;
    //velocity can be -1 or 1 for x and y
};


void ball_create(struct Ball* ball, int startpos_x, int startpos_y);
void ball_move(struct Ball* ball);
bool ball_detect_collision(struct Ball* ball, struct Paddle *paddle_left, struct Paddle *paddle_right);    //returns true if either player scores

#endif