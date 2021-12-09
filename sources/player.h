#ifndef PLAYER_H
#define PLAYER_H

#include"raylib.h"

typedef struct s_game s_game;

typedef struct s_player {
    Vector2 position;
    Vector2 velocity;
    float horizontal_speed;
    Shader shader; // custom shader for player
    Texture texture;
    Sound slurp_sound;
    int water_level_loc;
    bool can_grab_water;
    bool has_water;
    int time_location;
    float water_level;
    // Physics
    float mass;
    float air_resistance;
} s_player;

void UpdatePlayer(s_player* player, struct s_game* game);
void RenderPlayer(s_player* player);

#endif