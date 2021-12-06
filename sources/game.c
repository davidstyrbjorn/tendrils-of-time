#include"game.h"

#include<stdlib.h>

#include"raylib.h"
#include"rlgl.h"
#include"raymath.h"

#include"external/glad.h"

#include"tree.h"
#include"attacker.h"
#include"lsystem.h"

void StartGame(s_game* game){
    game->game_state = PLAYING;

    InitWindow(game->window_size.x, game->window_size.y, game->title);
    InitAudioDevice();
    SetMasterVolume(1.0f);

    SetTargetFPS(144);
    RunGame(game); // Go further into it by starting the game loop
}

void RunGame(s_game* game) {
    // Setup post-processing stuff
    game->pp_shader = LoadShader(0, TextFormat(ASSETS_PATH"postprocessing.glsl", 330)); // Load bloom shader
    game->framebuffer_texture = LoadRenderTexture(game->window_size.x, game->window_size.y);

    int loc = GetShaderLocation(game->pp_shader, "pixel_w");
    //SetShaderValue(game->pp_shader, loc, 1, SHADER_UNIFORM_FLOAT);
    game->pp_shader.locs[0] = loc;
    // TODO(dave): Fix this crashes the game?
    // SetShaderValue(game->pp_shader, 0, (const void*)1, SHADER_UNIFORM_FLOAT);

    // Create background shader and setup background rectangle
    game->background_shader = LoadShader(0, TextFormat(ASSETS_PATH"background.glsl", 330));
    Image imBlank = GenImageColor(game->window_size.x, game->window_size.y, BLANK);
    game->background_texture = LoadTextureFromImage(imBlank);  // Load blank texture to fill on shader
    UnloadImage(imBlank);
    game->time_location = glGetUniformLocation(game->background_shader.id, "time");

    // Create the ground object
    game->ground.width = game->window_size.x*0.8;
    game->ground.height = game->window_size.y * 0.1f;
    game->ground.x = 0;
    game->ground.y = game->window_size.y-game->ground.height;

    // Setup camera
    game->camera.zoom = 1.0f;

    // Do object-based tree generation
    CreateTree(&game->tree, (Vector2){0.0, -game->ground.height});

    // Generate grass
    game->grass.density = 15;
    game->grass.origin = (Vector2){0, game->window_size.y-game->ground.height};
    game->grass.horizontal_span = game->window_size.x*0.8;
    game->grass.height = 10;
    game->grass.height_variation = 2.5;
    game->grass.triangle_span = 4;
    game->grass.blow_frequency = 1;
    PopulateGrassField(&game->grass);

    // Create the pond
    game->pond.origin = (Vector2){game->window_size.x*0.8, game->window_size.y-game->ground.height};
    game->pond.width = game->window_size.x*0.2;
    CreatePond(&game->pond);

    // Create player
    float player_height = 80;
    float player_width = 40;
    game->player.rect = (Rectangle){100, game->window_size.y - game->ground.height - player_height, player_width, player_height};
    game->player.color = (Color){200, 100, 200, 255}; 
    game->player.horizontal_speed = 12500;
    game->player.mass = 5;
    game->player.air_resistance = 75;

    // Setup other
    game->second_counter = 0;
    game->bg_music = LoadMusicStream(ASSETS_PATH"tree_game.mp3");
    PlayMusicStream(game->bg_music);

    while(!WindowShouldClose()){
        // Update input related stuff
        InputGame(game); 

        UpdateMusicStream(game->bg_music); 
        
        switch(game->game_state){
            case MENU:
                RunMenu(game);
                break;
            case PLAYING:
                RunPlaying(game);
                break;
            case PAUSED:
                RunPaused(game);
                break;
            default:
                printf("Invalid game state!!!!");
                CloseWindow();
                break;
        }
    }

    //TODO(david): Make sure all memeory is deallocated please and thank you!
    UnloadMusicStream(game->bg_music);
    UnloadShader(game->pp_shader);
    UnloadShader(game->background_shader);
    DestructTree(&game->tree);
    UnloadPond(&game->pond);

    CloseAudioDevice();
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
    if(IsKeyPressed(KEY_M)){
        if(IsMusicStreamPlaying(game->bg_music)){
            PauseMusicStream(game->bg_music);
        }else{
            ResumeMusicStream(game->bg_music);
        }
    }     

    if(IsKeyDown(KEY_LEFT)){
        game->camera.offset.x -= 100 * GetFrameTime();
    }else if(IsKeyDown(KEY_RIGHT)){
        game->camera.offset.x += 100 * GetFrameTime();
    }

    if(IsKeyDown(KEY_Z)){
        game->camera.zoom += 1.f * GetFrameTime();
    } else if(IsKeyDown(KEY_X)){
        game->camera.zoom -= 1.f * GetFrameTime();
    }
}

/* RUNNERS */

void RunMenu(s_game* game){

}

void RunPaused(s_game* game){

}

void RunPlaying(s_game* game){
    // for(int i = 0; i < MAX_ATTACKERS; i++){
    //     UpdateAttacker(&game->tree, &game->attackers[i]);
    // }

    // Spawn attackers
    // if(GetTime() - game->second_counter >= 3){
    //     game->second_counter = GetTime();
    //     SpawnAttackers(game, 2);
    // }

    UpdatePlayer(&game->player, game);
    UpdateTree(&game->tree);

    // Update time for background shader
    float time = (float)GetTime();
    glUseProgram(game->background_shader.id);
    glUniform1f(game->time_location, time);

    glUseProgram(game->pond.shader.id);
    glUniform1f(game->pond.time_location, time);

    glUseProgram(0);

    BeginTextureMode(game->framebuffer_texture); // Enable so we draw to the framebuffer texture!
    BeginMode2D(game->camera);

        ClearBackground(SKYBLUE);

        // Render background
        BeginShaderMode(game->background_shader);
            DrawTexture(game->background_texture, 0, 0, WHITE);
        EndShaderMode();

        // Render pond
        RenderPond(&game->pond);

        /* Draws the basis vectors from 0, 0 on x,y axis */
        //DrawCoordinateAxis();

        // Render ground
        DrawRectangleGradientV(
            game->ground.x, game->ground.y, game->ground.width, game->ground.height,
            GREEN, BROWN
        );

        // Renders the tree object
        RenderTree(&game->tree);
        // Render grass
        RenderGrass(&game->grass);

        // Render player
        RenderPlayer(&game->player);

        // Render attackers
        for(int i = 0; i < MAX_ATTACKERS; i++){
            RenderAttacker(&game->attackers[i]);
        }
        
    EndMode2D();
    EndTextureMode(); // End framebuffer texture

    BeginDrawing(); // Draw the actual post processed framebuffer

        ClearBackground(WHITE);

        BeginShaderMode(game->pp_shader);
            // Flip the y due to how the default opengl coordinates work (left-bottom)
            DrawTextureRec(game->framebuffer_texture.texture, (Rectangle){0, 0, (float)+game->framebuffer_texture.texture.width, (float)-game->framebuffer_texture.texture.height}, (Vector2){0.0, 0.0}, WHITE);
        EndShaderMode();

    EndDrawing();
}