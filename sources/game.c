#include"game.h"

#include<stdlib.h>

#include"raylib.h"
#include"raymath.h"
#include"rlgl.h"

#include"tree.h"
#include"attacker.h"
#include"lsystem.h"

// L-System rule
void StartGame(s_game* game){
    InitWindow(game->window_size.x, game->window_size.y, game->title);
    
    // Setup camera
    game->camera.zoom = 1.0f;

    // Do object-based tree generation
    CreateTree(&game->tree);

    // Setup 
    game->second_counter = 0;

    SetTargetFPS(60);
    RunGame(game); // Go further into it by starting the game loop
}

void RunGame(s_game* game) {
    // Setup post-processing stuff
    Shader pp_shader = LoadShader(0, TextFormat(ASSETS_PATH"postprocessing.glsl", 330)); // Load bloom shader
    RenderTexture2D framebuffer_texture = LoadRenderTexture(game->window_size.x, game->window_size.y);

    while(!WindowShouldClose()){

        // Update input related stuff
        InputGame(game);
        for(int i = 0; i < MAX_ATTACKERS; i++){
            UpdateAttacker(&game->tree, &game->attackers[i]);
        }

        // Spawn attackers
        if(GetTime() - game->second_counter >= 3){
            game->second_counter = GetTime();
            SpawnAttackers(game, 2);
        }

        BeginTextureMode(framebuffer_texture); // Enable so we draw to the framebuffer texture!
        BeginMode2D(game->camera);

            ClearBackground(SKYBLUE);

            /* Draws the basis vectors from 0, 0 on x,y axis */
            DrawCoordinateAxis();

            // RecursiveTreeDraw(game->fractal_tree_start_length, 
            // game->fractal_tree_start_length, 
            // game->fractal_tree_angle);
            //DrawLSystem(game->l_string, 14, 90, 1.001);

            RenderTree(&game->tree);

            // Render attackers
            for(int i = 0; i < MAX_ATTACKERS; i++){
                RenderAttacker(&game->attackers[i]);
            }
            
        EndMode2D();
        EndTextureMode(); // End framebuffer texture

        BeginDrawing(); // Draw the actual post processed framebuffer

            ClearBackground(WHITE);
            BeginShaderMode(pp_shader);
                // Flip the y due to how the default opengl coordinates work (left-bottom)
                DrawTextureRec(framebuffer_texture.texture, (Rectangle){0, 0, (float)+framebuffer_texture.texture.width, (float)-framebuffer_texture.texture.height}, (Vector2){0.0, 0.0}, WHITE);
            EndShaderMode();

        EndDrawing();
    }

    CloseWindow();

}

void EndGame(s_game* game) {

}

void DrawCoordinateAxis() {
    Vector2 origin = (Vector2){ 0.0f, 0.0f };
    float axisThickness = 8.0f;
    float axisScale = 500.0f;
    Color axisColor = RED;

    DrawLineEx(origin, Vector2Add(origin, (Vector2) { axisScale, 0.0f }), axisThickness, axisColor); // X-axis
    DrawLineEx(origin, Vector2Add(origin, (Vector2) { 0.0f, axisScale }), axisThickness, axisColor); // Y-axis
}

void InputGame(s_game* game) {
    Vector2 moveVector = { 0 };

    bool boosted = IsKeyDown(KEY_LEFT_SHIFT);
    
    if (IsKeyDown(KEY_D)) {
        moveVector.x--;
    }
    else if (IsKeyDown(KEY_A)) {
        moveVector.x++;
    }

    if (IsKeyDown(KEY_S)) {
        moveVector.y--;
    }
    else if (IsKeyDown(KEY_W)) {
        moveVector.y++;
    }

    float cameraSpeed = 140.f;
    if(boosted) cameraSpeed *= 3.f;

    // Scale with delta time
    moveVector = Vector2Scale(moveVector, cameraSpeed*GetFrameTime());

    // Move camera
    game->camera.offset = Vector2Add(game->camera.offset, moveVector);

    // Angle input thing
    if(IsKeyDown(KEY_UP)){
        game->fractal_tree_angle += 10.0f * GetFrameTime();
    }else if(IsKeyDown(KEY_DOWN)){
        game->fractal_tree_angle += -10.0f * GetFrameTime();
    }   

    if(IsKeyDown(KEY_Z)){
        game->camera.zoom += 1.f * GetFrameTime();
    } else if(IsKeyDown(KEY_X)){
        game->camera.zoom -= 1.f * GetFrameTime();
    }

}