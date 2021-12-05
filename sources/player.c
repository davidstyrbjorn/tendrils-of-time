#include"player.h"

#include"raymath.h"

#define GRAVITY -9.82f

void UpdatePlayer(s_player* player){
    
    Vector2 acceleration = (Vector2){0, -GRAVITY};
    float dt = GetFrameTime();
    
    // Check for input
    if(IsKeyDown(KEY_D)){
        acceleration.x += player->horizontal_speed;
    }
    else if(IsKeyDown(KEY_A)){
        acceleration.x += player->horizontal_speed;
    }

    // Euler 
    Vector2 velocity = {0}; // Previous velocity is zero?
    velocity = Vector2Scale(acceleration, dt);
    player->position = Vector2Add(player->position, Vector2Scale(velocity, dt));
}