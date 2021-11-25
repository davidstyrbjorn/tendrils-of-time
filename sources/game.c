#include"game.h"

#include<stdlib.h>

#include"raylib.h"
#include"raymath.h"
#include"rlgl.h"

#include"tree.h"
#include"lsystem.h"

// L-System rule
char* test(char c){
    if(c == 'F') return "FF";
    if(c == 'X') return "F-[[X+]+X+]+F[+FX]+-X";
    return NULL;
}

char* quadratic_gosper(char c){
    if(c == 'X')
        return "XFX-YF-YF+FX+FX-YF-YFFX+YF+FXFXYF-FX+YF+FXFX+YF-FXYF-YF-FX+FX+YFYF-";
    if(c == 'Y')
        return "+FXFX-YF-YF+FX+FXYF+FX-YFYF-FX-YF+FXYFYF-FX-YFFX+FX+YF-YF-FX+FX+YFY";
    return NULL;
}

void StartGame(s_game* game){
    InitWindow(game->window_size.x, game->window_size.y, game->title);
    
    // Setup camera
    game->camera.zoom = 1.0f;

    // Test L-System
    char* x = "-YF";
    game->l_string = LSystemStart(x, 2, quadratic_gosper);
    //printf("STRING:%s\n", game->l_string);

    // Do object-based tree generation
    // Setup initial branch
    game->base_length = 100.0f;
    game->base_thickness = 15.0f;
    game->branches[0].done = false;
    game->branches[0].start = (Vector2){0.0, 0.0};
    game->branches[0].length = game->base_length;
    game->branches[0].end = (Vector2){0.0, -game->base_length};

    game->branch_count = 1;
    game->iteration_levels = 8;
    for(int i = 0; i < game->iteration_levels; i++){
        int frozen_branch_count = game->branch_count;
        for(int j = 0; j < frozen_branch_count; j++){
            if(!game->branches[j].done){
                // Spawn 2 branches and insert to the end of branch list
                game->branches[j].done = true;
                game->branches[game->branch_count] = SpawnBranch(&game->branches[j], 1);
                game->branch_count++;
                game->branches[game->branch_count] = SpawnBranch(&game->branches[j], -1);
                game->branch_count++;
            }
        }
    }

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

        BeginTextureMode(framebuffer_texture); // Enable so we draw to the framebuffer texture!
        BeginMode2D(game->camera);

            ClearBackground(SKYBLUE);

            /* Draws the basis vectors from 0, 0 on x,y axis */
            //DrawCoordinateAxis();

            rlPushMatrix();
            rlTranslatef(game->window_size.x/2, game->window_size.y, 0); // Translate to middle and bottom 
            
            // RecursiveTreeDraw(game->fractal_tree_start_length, 
            // game->fractal_tree_start_length, 
            // game->fractal_tree_angle);
            //DrawLSystem(game->l_string, 14, 90, 1.001);

            DrawObjectTree(game);

            rlPopMatrix();
            
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

// TODO: Move this to the tree file, makes more sense since alot of logic of renderin tree-stuff goes here
void DrawObjectTree(s_game* game){
    // Draw the object tree branch by branch
    for(int i = 0; i < game->branch_count; i++){
            // Branch thickness based on length of the branch, see game->base_thickness & length
            s_branch b = game->branches[i];
            float ratio = b.length / game->base_length;
            DrawLineEx(b.start, b.end, game->base_thickness*ratio, BROWN);
            // This is true if we're on a branch that has no children
            if(!b.done){
                Vector2 wind = {0};
                float temp = b.dynamics.wind_offset + b.dynamics.wind_frequency*GetTime();
                wind.x = cos(temp) * b.dynamics.wind_amplitude;
                //wind.y = sin(temp) * b.dynamics.wind_amplitude;
                DrawCircle(b.end.x + wind.x, b.end.y + wind.y, 20, ColorAlpha(GREEN, 0.8f));
            }
        }
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