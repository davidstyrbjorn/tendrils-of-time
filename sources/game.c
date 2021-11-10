#include"game.h"

#include"raylib.h"

void StartGame(s_game* game){
    InitWindow(game->window_size.x, game->window_size.y, game->title);

    RunGame(game); // Go further into it by starting the game loop
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