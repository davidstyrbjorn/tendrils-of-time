#include"player.h"

#include"raymath.h"

#include<stdio.h>

#include"game.h"

#define GRAVITY -2000

void UpdatePlayer(s_player* player, s_game* game){
    
    Vector2 force = (Vector2){0, 0};
    float dt = GetFrameTime();
    
    // Check for input
    if(IsKeyDown(KEY_D)){
        force.x += player->horizontal_speed;
    }
    else if(IsKeyDown(KEY_A)){
        force.x += -player->horizontal_speed;
    }

    // Air resistance proportional to player velocity
    Vector2 f_air = Vector2Scale(player->velocity, -player->air_resistance);

    // Do final force
    Vector2 final_force = force;
    final_force = Vector2Add(final_force, f_air);

    Vector2 position = (Vector2){player->rect.x, player->rect.y};
    
    // Euler 
    position = Vector2Add(position, Vector2Scale(player->velocity, dt));
    Vector2 temp = Vector2Scale(final_force, dt / player->mass);
    
    player->velocity = Vector2Add(player->velocity, temp);

    player->rect.x = position.x; player->rect.y = position.y;
}

void RenderPlayer(s_player* player){

    DrawRectanglePro(player->rect, (Vector2){0,0}, 0, player->color);
}