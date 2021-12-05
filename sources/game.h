#ifndef GAME_H
#define GAME_H

#include"raylib.h"
#include"tree.h"
#include"attacker.h"
#include"constants.h"
#include"player.h"

typedef enum GAME_STATE {
    MENU,
    PLAYING,
    PAUSED
} GAME_STATE;

typedef struct s_game {
    GAME_STATE game_state;
    Shader pp_shader;
    RenderTexture2D framebuffer_texture;
    s_player player;
    Rectangle ground;
    // Some game config data
    const char* title;
    Vector2 window_size;
    Camera2D camera;
    // Fractal tree data
    float fractal_tree_angle;
    int fractal_tree_start_length;
    // L system data
    char* l_string;
    // Object based tree rendering
    s_tree tree;
    // Attacker data
    s_attacker attackers[MAX_ATTACKERS];
    int second_counter;
} s_game;

// Declare game related functions
void StartGame(s_game* game);
void RunGame(s_game* game);
void EndGame(s_game* game);
void InputGame(s_game* game);

void RunMenu(s_game* game);
void RunPaused(s_game* game);
void RunPlaying(s_game* game);

void SpawnAttackers(s_game* game, int how_many);

/* Moves these to a rendering.h file */
void DrawCoordinateAxis();

#endif