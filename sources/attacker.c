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

    // Check for mouse click on enemey
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector2 mousePosition = GetMousePosition();
        // Is point inside us?
        if(Vector2Distance(mousePosition, attacker->position) < attacker->radius*1.5f){
            cvector_push_back(game->available_attacker_indices, attacker->idx);
            attacker->enabled = false;
            if(attacker->player_taken){
                attacker->player_taken = false;
                player->position_state = NONE;
            }
            // SFX & VFX
            PlaySound(game->enemy_die_sfx);
            StartCameraShake(game, 0.3, 6);
            CreateAttackText(game, attacker->position, "HIT!", GREEN);
            return;
        }
    }

    // If this attacker has the player, move to new goal positon and take the player with us
    if(attacker->player_taken){
        Vector2 goalPosition = (Vector2){0, -200};
        Vector2 newPosition = Vector2MoveTowards(attacker->position, goalPosition, attacker->speed*GetFrameTime());
        attacker->position = newPosition;
        player->position = (Vector2){attacker->position.x-player->texture.width/2, attacker->position.y};
    }

    // If the player is already taken don't move towards!
    if(player->position_state == TAKEN_BY_ATTACKER) return; 
    // Move attacker towards player
    Vector2 newPosition = Vector2MoveTowards(attacker->position, playerPosition, attacker->speed*GetFrameTime());
    attacker->position = newPosition;
    // Are we at the player position?
    if(Vector2Distance(playerPosition, attacker->position) < 1){
        // The attacker has reached the goal
        attacker->player_taken = true;
        player->position_state = TAKEN_BY_ATTACKER;
        StartCameraShake(game, 0.4f, 8);
        CreateAttackText(game, attacker->position, "TAKEN!!!", RED);
        return;
    }
}

// Setup the given attacker on a new leaf
s_attacker SpawnAttacker(s_game* game, int idx){
    s_attacker attacker = {0};
    attacker.idx = idx; // Save this for later
    Vector2 speed = GetEnemySpeed(game);
    attacker.speed = GetRandomValue(speed.x, speed.y);
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
    DrawCircle(attacker->position.x, attacker->position.y, attacker->radius, RED);
}

Vector2 GetEnemySpeed(s_game* game){
    float t = game->gameplay_timer;
    if(t >= 60){
        return (Vector2){200.0f, 200.0f};
    }
    else if(t >= 40){
        return (Vector2){120.0f, 180.0f};
    }
    else if(t >= 30){
        return (Vector2){100.0f, 130.0f};
    }
    else if(t >= 20){
        return (Vector2){70.0f, 100.0f};
    }
    else{
        return (Vector2){40.0f, 80.0f};
    }
}

float GetEnemySpawnTime(s_game* game){
    // Return a time based on the current play time for game
    float t = game->gameplay_timer;
    if(t >= 40){
        return 1;
    }
    else if(t >= 30){
        return 2;
    }
    else if(t >= 20){
        return 3.5;
    }
    else{
        return 4.5;
    }
}