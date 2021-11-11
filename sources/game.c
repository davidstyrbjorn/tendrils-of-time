#include"game.h"

#include"raylib.h"
#include"rlgl.h"

void StartGame(s_game* game){
    InitWindow(game->window_size.x, game->window_size.y, game->title);

    RunGame(game); // Go further into it by starting the game loop

    // TODO: Can we do shader stuff here? Setup a quad which covers the screen?
    // Try out the rlgl abstraction framework
}

void RunGame(s_game* game) {
    while(!WindowShouldClose()){
        BeginDrawing();

        ClearBackground(SKYBLUE);

        EndDrawing();
    }

    CloseWindow();
}

void EndGame(s_game* game) {
}