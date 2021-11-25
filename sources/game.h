#ifndef GAME_H
#define GAME_H

#include"raylib.h"
#include"tree.h"

#define MAX_BRANCHES 1028

typedef struct s_game {
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
    s_branch branches[MAX_BRANCHES];
    int iteration_levels;
    int branch_count;
    float base_thickness;
    float base_length;
} s_game;

// Declare game related functions
void StartGame(s_game* game);
void RunGame(s_game* game);
void EndGame(s_game* game);
void InputGame(s_game* game);
void DrawObjectTree(s_game* game);

/* Moves these to a rendering.h file */
void DrawCoordinateAxis();

#endif