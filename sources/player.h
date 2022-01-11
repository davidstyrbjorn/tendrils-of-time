#ifndef PLAYER_H
#define PLAYER_H

#include"raylib.h"

typedef struct s_game;
typedef struct s_seed;

typedef enum PLAYER_POSITION_STATES {
    TREE,
    POND,
    TAKEN_BY_ATTACKER,
    NONE
} PLAYER_POSITION_STATES;

typedef struct s_player {
    Vector2 position;
    Vector2 velocity;
    float horizontal_speed;
    Shader shader; // custom shader for player
    Texture texture;
    Sound slurp_sound;
    Sound eat_seed_sound;
    int water_level_loc;
    bool has_water;
    int time_location;
    float water_level;
    int number_of_seeds_eaten;
    PLAYER_POSITION_STATES position_state;
    // Physics
    float mass;
    float air_resistance;
} s_player;

void UpdatePlayer(s_player* player, struct s_game* game);
void RenderPlayer(s_player* player, struct s_game* game);
void EatSeed(s_player* player, struct s_seed* seed, struct s_game* game);

#endif