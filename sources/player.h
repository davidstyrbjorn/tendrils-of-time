#ifndef PLAYER_H
#define PLAYER_H

#include"raylib.h"

typedef struct s_player {
    Vector2 position;
    Vector2 velocity;
    float horizontal_speed;
} s_player;

void UpdatePlayer(s_player* player);
void RenderPlayer(s_player* player);

#endif