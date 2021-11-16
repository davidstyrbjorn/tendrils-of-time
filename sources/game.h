#ifndef GAME_H
#define GAME_H

#include"raylib.h"

typedef struct s_game {
    // Some game config data
    const char* title;
    Vector2 window_size;
    Camera2D camera;

    // Fractal tree data
    int fractal_tree_start_length;
    float fractal_tree_angle;

} s_game;

// Declare game related functions
void StartGame(s_game* game);
void RunGame(s_game* game);
void EndGame(s_game* game);
void InputGame(s_game* game);

/* Moves these to a rendering.h file */
void DrawCoordinateAxis();

#endif