#ifndef PLAYER_H
#define PLAYER_H

#include"raylib.h"

typedef struct s_game s_game;

typedef struct s_player {
    Rectangle rect;
    Vector2 velocity;
    float horizontal_speed;
    Color color;
    // Physics
    float mass;
    float air_resistance;
} s_player;

void UpdatePlayer(s_player* player, struct s_game* game);
void RenderPlayer(s_player* player);

#endif