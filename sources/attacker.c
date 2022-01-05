#include"attacker.h"

#include"raylib.h"
#include"raymath.h"

#include"cvector.h"
#include"utility.h"
#include"tree.h" // s_leaf & s_tree
#include"player.h"
#include"game.h" // s_game

#include<stdlib.h>

#define SOME_THRESHOLD 0.1

void UpdateAttacker(s_game* game, s_attacker* attacker){
    if(Vector2Distance(attacker->player->position, attacker->position) < 0.2f){
        // The attacker has reached the goal
        cvector_push_back(game->available_attacker_indices, attacker->idx);
        attacker->enabled = false;
        return;
    }
    // Move attacker towards leaf
    Vector2 newPosition = Vector2MoveTowards(attacker->position, attacker->player->position, attacker->speed*GetFrameTime());
    attacker->position = newPosition;
}

// Setup the given attacker on a new leaf
s_attacker SpawnAttacker(s_game* game, int idx){
    s_attacker attacker = {0};
    attacker.idx = idx; // Save this for later
    attacker.speed = GetRandomValue(40.0f, 80.0f);
    // Position
    attacker.position.y = GetRandomValue(0, GetScreenHeight());
    if(GetRandomFloatValue01() > 0.5f){
        attacker.position.x = -20;
    }else{
        attacker.position.x = GetScreenWidth() + 20;
    }
    attacker.enabled = true; // Enabled on spawn
    attacker.player = &game->player;
    return attacker;
}

void RenderAttacker(s_attacker* attacker){
    if(!attacker->enabled) return;
    DrawCircle(attacker->position.x, attacker->position.y, 10, RED);
}