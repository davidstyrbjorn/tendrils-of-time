#include"attacker.h"

#include"raylib.h"
#include"raymath.h"

#include"cvector.h"
#include"utility.h"
#include"tree.h" // s_leaf & s_tree
#include"player.h"
#include"game.h" // s_game

#include<stdlib.h>
#include<stdio.h>

#define SOME_THRESHOLD 0.1

void UpdateAttacker(s_game* game, s_attacker* attacker){
    s_player* player = &game->player;
    // Head of the player
    Vector2 playerPosition = Vector2Add(player->position, (Vector2){player->texture.width/2, 0}); 

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector2 mousePosition = GetMousePosition();
        // Is point inside us?
        if(Vector2Distance(mousePosition, attacker->position) < attacker->radius){
            cvector_push_back(game->available_attacker_indices, attacker->idx);
            attacker->enabled = false;
            PlaySound(game->enemy_die_sfx);
        }
    }

    // If the player is already taken don't move towards!
    if(player->position_state == TAKEN_BY_ENEMY) return; 
    // Move attacker towards player
    Vector2 newPosition = Vector2MoveTowards(attacker->position, playerPosition, attacker->speed*GetFrameTime());
    attacker->position = newPosition;
    // Are we at the player position?
    if(Vector2Distance(playerPosition, attacker->position) < 0.2f){
        // The attacker has reached the goal
        cvector_push_back(game->available_attacker_indices, attacker->idx);
        attacker->enabled = false;
        StartCameraShake(game, 0.4f, 8);
        return;
    }
}

// Setup the given attacker on a new leaf
s_attacker SpawnAttacker(s_game* game, int idx){
    s_attacker attacker = {0};
    attacker.idx = idx; // Save this for later
    attacker.speed = GetRandomValue(40.0f, 80.0f);
    attacker.radius = 15;
    // Position
    attacker.position.y = GetRandomValue(0, GetScreenHeight());
    if(GetRandomFloatValue01() > 0.5f){
        attacker.position.x = -20;
    }else{
        attacker.position.x = GetScreenWidth() + 20;
    }
    attacker.enabled = true; // Enabled on spawn
    return attacker;
}

void RenderAttacker(s_attacker* attacker){
    if(!attacker->enabled) return;
    DrawCircle(attacker->position.x, attacker->position.y, attacker->radius, RED);
}