#include "raylib.h"

// Include game, which basically handles EVERYTHING, keep this file minimum
#include"game.h"

int main(void)
{

    // Basic configuration for game
    s_game game = {0};
    game.title = "Tree. =_=$€";
    game.window_size = (Vector2){800, 600};

    // Entrypoint
    StartGame(&game);

    return 0;
}
