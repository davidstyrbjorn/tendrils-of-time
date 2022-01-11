#include"seed.h"

#include"game.h"
#include<stdio.h>

const float SEED_SPEED = 100;

void CreateSeed(s_tree* tree, Vector2 position){
    s_seed seed = {0};
    seed.position = position;
    seed.enabled = true;

    // find available spot for seed
    for(int i = 0; i < MAX_SEED_COUNT; i++){
        if(!tree->seeds[i].enabled){
            tree->seeds[i] = seed;
            break;
        }
    }
}

void RenderSeed(s_seed* seed){
    DrawCircleV(seed->position, 5, GREEN);
}

void UpdateSeed(s_game* game, s_seed* seed){
    float dt = GetFrameTime();
    seed->position.y += SEED_SPEED * dt; 

    // check if seed has hit ground
    if(seed->position.y > GetScreenHeight() - game->ground.height){
        seed->enabled = false;
    }

    // Check for collision with player
    bool inside_player = CheckCollisionPointRec(seed->position, (Rectangle){
        game->player.position.x, game->player.position.y,
        game->player.texture.width, game->player.texture.height
    });
    if(inside_player){
        seed->enabled = false;
        EatSeed(&game->player, seed, game);
    }   
}