#include"game.h"

#include<stdlib.h>

#include"raylib.h"
#include"rlgl.h"
#include"raymath.h"

#include"external/glad.h"

#include"tree.h"
#include"attacker.h"
#include"lsystem.h"
#include"cvector.h"
#include"utility.h"

void StartGame(s_game* game){
    game->game_state = MENU;

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
    Image imBlank = GenImageColor(game->window_size.x, game->window_size.y*2, BLANK);
    game->background_texture = LoadTextureFromImage(imBlank);  // Load blank texture to fill on shader
    UnloadImage(imBlank);
    game->time_location = glGetUniformLocation(game->background_shader.id, "time");
    game->water_meter_location = glGetUniformLocation(game->background_shader.id, "water_meter");

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

    // Create the pond & water meter
    game->pond.origin = (Vector2){game->window_size.x*0.8, game->window_size.y-game->ground.height*0.8};    
    game->pond.width = game->window_size.x*0.2;
    CreatePond(&game->pond);
    CreateWaterMeter(&game->water_meter);

    // Setup player
    float player_height = 80;
    float player_width = 40;
    game->player.slurp_sound = LoadSound(ASSETS_PATH"slurp.wav");
    game->player.eat_seed_sound = LoadSound(ASSETS_PATH"eat_seed.wav");
    game->player.number_of_seeds_eaten = 0;
    game->player.has_water = false;
    game->player.position = (Vector2){100, game->window_size.y - game->ground.height - player_height};
    game->player.horizontal_speed = 12500;
    game->player.mass = 5;
    game->player.air_resistance = 75;
    imBlank = GenImageColor(player_width, player_height, BLANK);
    game->player.texture = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);
    game->player.shader = LoadShader(0, TextFormat(ASSETS_PATH"player.glsl", 330));
    game->player.time_location = glGetUniformLocation(game->player.shader.id, "time");
    game->player.water_level = 0.0f;   
    game->player.water_level_loc = glGetUniformLocation(game->player.shader.id, "water_level");

    // Setup other
    game->second_counter = 0;
    game->camera_shake_timer = 0;
    game->bg_music = LoadMusicStream(ASSETS_PATH"tree-song.mp3");
    game->menu_music = LoadMusicStream(ASSETS_PATH"menu.mp3");
    game->game_over_music = LoadMusicStream(ASSETS_PATH"game-over.mp3");
    PlayMusicStream(game->menu_music);
    game->enemy_die_sfx = LoadSound(ASSETS_PATH"enemy_dead.wav");
    game->gameplay_timer = 0.0f;
    
    // Fill the available attacker indices with all indices
    game->available_attacker_indices = NULL;
    for(int i = 0; i < MAX_ATTACKERS; i++) { cvector_push_back(game->available_attacker_indices, i); }
    // Loop through alot of lists and set enabled to false
    for(int i = 0; i < MAX_ATTACKERS; i++) { game->attackers[i].enabled = false; }
    for(int i = 0; i < MAX_ATTACK_TEXT; i++) { game->attack_text[i].enabled = false; }

    while(!WindowShouldClose()){
        // Update input related stuff
        InputGame(game); 

        // Needs to be called in order for music to work
        UpdateMusicStream(game->bg_music); 
        UpdateMusicStream(game->menu_music);
        UpdateMusicStream(game->game_over_music);
        
        // Runs main game loop
        GameplayLoop(game);
    }

    //TODO(david): Make sure all memeory is deallocated please and thank you!
    UnloadMusicStream(game->bg_music);
    UnloadMusicStream(game->menu_music);
    UnloadShader(game->pp_shader);
    UnloadShader(game->background_shader);
    UnloadShader(game->player.shader);
    DestructTree(&game->tree);
    UnloadPond(&game->pond);
    UnloadSound(game->player.slurp_sound);
    UnloadSound(game->tree.hurt_sound);
    UnloadTexture(game->player.texture);

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
    // Mute background music toggle
    if(IsKeyPressed(KEY_M)){
        if(IsMusicStreamPlaying(game->bg_music)){
            PauseMusicStream(game->bg_music);
        }else{
            ResumeMusicStream(game->bg_music);
        }
    }     

    // if(IsKeyDown(KEY_LEFT)){
    //     game->camera.offset.x -= 100 * GetFrameTime();
    // }else if(IsKeyDown(KEY_RIGHT)){
    //     game->camera.offset.x += 100 * GetFrameTime();
    // }

    // if(IsKeyDown(KEY_Z)){
    //     game->camera.zoom += 1.f * GetFrameTime();
    // } else if(IsKeyDown(KEY_X)){
    //     game->camera.zoom -= 1.f * GetFrameTime();
    // }
}

/* RUNNERS */

void GameplayLoop(s_game* game){
    if(game->in_transition){
        UpdateTransition(game);
    }
    switch(game->game_state){
        case MENU:
            UpdateMenu(game);
            break;
        case PLAYING:
            UpdatePlaying(game);
            break;
        case PAUSED:
            UpdatePaused(game);
            break;
    }

    // Update uniform
    float time = (float)GetTime();
    glUseProgram(game->background_shader.id);
    glUniform1f(game->time_location, time);
    glUniform1f(game->water_meter_location, game->tree.water_counter);

    glUseProgram(game->pond.shader.id);
    glUniform1f(game->pond.time_location, time);
    glUniform1f(game->pond.water_location, game->pond.water_level);
    
    glUseProgram(game->player.shader.id);
    glUniform1f(game->player.time_location, time);
    glUniform1f(game->player.water_level_loc, game->player.water_level);
    
    glUseProgram(game->water_meter.shader.id);
    glUniform1f(game->water_meter.water_level_location, game->tree.water_counter);
    glUniform1f(game->water_meter.shader.id, time);
    
    glUseProgram(0);

    BeginTextureMode(game->framebuffer_texture); // Enable so we draw to the framebuffer texture!
    BeginMode2D(game->camera);

        ClearBackground(SKYBLUE);

        // Render background with the background shader
        BeginShaderMode(game->background_shader);
            DrawTexture(game->background_texture, 0, -600, WHITE);
        EndShaderMode();

        // Render pond
        RenderPond(&game->pond);

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
        RenderPlayer(&game->player, game);
        // Render attackers
        for(int i = 0; i < MAX_ATTACKERS; i++){
            if(!game->attackers[i].enabled) continue;
            RenderAttacker(&game->attackers[i]);
        }
        // Render attack text
        for(int i = 0; i < MAX_ATTACK_TEXT; i++){
            if(!game->attack_text[i].enabled) continue;
            RenderAttackText(&game->attack_text[i]);
        }
        RenderWaterMeter(game, &game->water_meter);
        
    EndMode2D();
    EndTextureMode(); // End framebuffer texture

    BeginDrawing(); // Draw the actual post processed framebuffer

        ClearBackground(WHITE);

        BeginShaderMode(game->pp_shader);
            // Flip the y due to how the default opengl coordinates work (left-bottom)
            DrawTextureRec(game->framebuffer_texture.texture, (Rectangle){0, 0, (float)+game->framebuffer_texture.texture.width, (float)-game->framebuffer_texture.texture.height}, (Vector2){0.0, 0.0}, WHITE);
        EndShaderMode();

        /* Draws the basis vectors from 0, 0 on x,y axis */
        //DrawCoordinateAxis();

        // Game state dependent renders outside of the post processing effect
        if(game->game_state == MENU){
            RenderMenu(game);
        }
        if(game->game_state == PAUSED){
            RenderPaused(game);
        }
        if(game->game_state == GAME_OVER){
            RenderGameOver(game);
        }

    EndDrawing();
}

void UpdateTransition(s_game* game){
    if(game->camera.offset.y > 0){
        game->camera.offset.y -= 500 * GetFrameTime();
    }else{
        game->in_transition = false;
        game->camera.offset.y = 0;
    }
}

void UpdatePlaying(s_game* game){
    // Take input for pausing
    if(IsKeyPressed(KEY_P)){
        game->game_state = PAUSED;
    }

    // update our gameplay timer
    game->gameplay_timer += 1.0f * GetFrameTime();

    // Update attackers
    for(int i = 0; i < MAX_ATTACKERS; i++){
        if(!game->attackers[i].enabled) continue;
        UpdateAttacker(game, &game->attackers[i]);
    }
    // Update attack text
    for(int i = 0; i < MAX_ATTACK_TEXT; i++){
        if(!game->attack_text[i].enabled) continue;
        UpdateAttackText(&game->attack_text[i]);
    }
    // Spawn attackers? Every 6 seconds
    if(GetTime() - game->second_counter >= GetEnemySpawnTime(game)){
        game->second_counter = GetTime();
        int _idx = game->available_attacker_indices[0];
        game->attackers[_idx] = SpawnAttacker(game, _idx);
        // vector_erase(game->available_attacker_indices, 0, 1);
        cvector_erase(game->available_attacker_indices, 0);
    }
    // Camera?
    if(game->camera_shake_timer > 0){
        UpdateCameraShake(game);
    }

    // Update player and tree
    UpdatePond(&game->pond);
    UpdatePlayer(&game->player, game);
    UpdateTree(&game->tree, game);
}

void UpdateMenu(s_game* game){
    game->camera.offset.y = 600;
    if(IsKeyPressed(KEY_SPACE)){
        game->game_state = PLAYING;
        game->in_transition = true;
        PlayMusicStream(game->bg_music);
        StopMusicStream(game->menu_music);
    }
}

void RenderMenu(s_game* game){
    DrawText("TENDRILS OF TIME", (game->window_size.x/5), 100, 48, WHITE);

    float x = sin(GetTime()*9);
    Color c = GREEN;
    if(x > 0) c = ColorAlpha(WHITE, 0.0f);
    DrawText("PRESS SPACE TO PLAY", (game->window_size.x/6), 250, 48, c);
}

void UpdatePaused(s_game* game){
    // Unpause
    if(IsKeyPressed(KEY_P)){
        game->game_state = PLAYING;
    }
}

void RenderGameOver(s_game* game){
    DrawText("YOUR TREE HAS DIED!", 30, 180, 48, RED);
    DrawText("GAME OVER!!!", 40, 220, 48, RED);
    int final_score = game->player.number_of_seeds_eaten * (int)game->gameplay_timer;
    char final_score_str[100];
    sprintf(final_score_str, "FINAL SCORE: %d", final_score);
    DrawText(final_score_str, 50, 360, 48, GREEN);
}

void RenderPaused(s_game* game){
    DrawText("PRESS P TO CONTINUE", (game->window_size.x/6), 175, 48, GREEN);
}

void StartCameraShake(s_game* game, float how_long, float strength){
    game->camera_shake_timer = how_long;
    game->camera_shake_strength = strength;
    game->camera_saved_offset = game->camera.offset;
}

void UpdateCameraShake(s_game* game){
    // Do the shaking
    Vector2 offset = (Vector2){GetRandomFloatValue01(), GetRandomFloatValue01()};
    offset = Vector2Scale(offset, game->camera_shake_strength);
    Vector2 newCameraOffset = Vector2Add(game->camera_saved_offset, offset);
    game->camera.offset = newCameraOffset; // Updates the actual offset of the camera

    // Decrease timer and reset if we're done
    game->camera_shake_timer -= 1.0f * GetFrameTime();
    if(game->camera_shake_timer <= 0){
        game->camera_shake_timer = 0;
        game->camera.offset = game->camera_saved_offset;
    }
}