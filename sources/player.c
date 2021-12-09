#include"player.h"

#include"raymath.h"
#include"external/glad.h"

#include<stdio.h>

#include"game.h"
#include"utility.h"

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
    // The player can grab water if we're at pond + we don't already have any water
    else if(IsKeyPressed(KEY_SPACE) && player->can_grab_water && !player->has_water){
        PlaySound(player->slurp_sound);
        player->has_water = true;
    }

    // Air resistance proportional to player velocity
    Vector2 f_air = Vector2Scale(player->velocity, -player->air_resistance);

    // Do final force
    Vector2 final_force = force;
    final_force = Vector2Add(final_force, f_air);

    Vector2 position = player->position;
    
    // Euler 
    position = Vector2Add(position, Vector2Scale(player->velocity, dt));
    Vector2 temp = Vector2Scale(final_force, dt / player->mass);
    
    player->velocity = Vector2Add(player->velocity, temp);

    // Can't move passed water, are we close to water? Then we can grab it
    if(position.x + player->texture.width > game->pond.origin.x){
        position.x = game->pond.origin.x - player->texture.width;
    }else if(position.x + player->texture.width > game->pond.origin.x - 50){
        player->can_grab_water = true;
    }else{
        player->can_grab_water = false;
    }

    player->position.x = position.x; player->position.y = position.y;

    // Update water level
    if(player->has_water && player->water_level < 1.0f){
        player->water_level = LinearInterpolate(player->water_level, 1.0f, 1.0f*dt);
    }
}

void RenderPlayer(s_player* player){
    // Render player with custom shader
    BeginShaderMode(player->shader);
        DrawTexture(player->texture, player->position.x, player->position.y, WHITE);
    EndShaderMode();

    // UI related
    if(player->can_grab_water && !player->has_water){
        DrawText("Press SPACE to grab water", 0, 0, 32, WHITE);
    }
}