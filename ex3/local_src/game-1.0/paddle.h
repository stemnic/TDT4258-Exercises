
#ifndef PADDLE_H
#define PADDLE_H

#define GAME_WIDTH 160
#define GAME_HEIGHT 120
#define PADDLE_WIDTH 10

struct Paddle{
    int pos_x, pos_y;
};

void paddle_create(struct Paddle* paddle, int start_pos_x);
void paddle_move_up(struct Paddle* paddle);
void paddle_move_down(struct Paddle* paddle);


#endif