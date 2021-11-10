#ifndef GAME_H
#define GAME_H

#include"raylib.h"

typedef struct s_game {
    // Some game config data
    const char* title;
    Vector2 window_size;

} s_game;

// Declare game related functions
void StartGame(s_game* game);
void RunGame(s_game* game);
void EndGame(s_game* game);

#endif