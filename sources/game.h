#ifndef GAME_H
#define GAME_H

#include"raylib.h"
#include"tree.h"
#include"attacker.h"
#include"constants.h"
#include"player.h"
#include"grass.h"
#include"pond.h"
#include"water_meter.h"
#include"attack_text.h"

typedef enum GAME_STATE {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
} GAME_STATE;

typedef struct s_game {
    GAME_STATE game_state;
    Shader pp_shader, background_shader;
    RenderTexture2D framebuffer_texture;
    s_player player;
    Rectangle ground;
    Texture background_texture;
    int time_location;
    int water_meter_location;
    s_pond pond;
    // Some game config data
    const char* title;
    Vector2 window_size;
    Camera2D camera;
    // Fractal tree data
    float fractal_tree_angle;
    int fractal_tree_start_length;
    // L system data
    char* l_string;
    // Object based tree rendering
    s_tree tree;
    s_grass grass;
    // Attacker data
    s_attacker attackers[MAX_ATTACKERS];
    int* available_attacker_indices;
    Sound enemy_die_sfx;
    s_attack_text attack_text[MAX_ATTACK_TEXT];
    int second_counter;
    float gameplay_timer;
    // Tha water meter
    s_water_meter water_meter;
    // Sound
    Music bg_music;
    Music menu_music;
    Music game_over_music;
    bool in_transition;
    float camera_shake_timer;
    float camera_shake_strength;
    Vector2 camera_saved_offset;
} s_game;

// Declare game related functions
void StartGame(s_game* game);
void RunGame(s_game* game);
void EndGame(s_game* game);
void InputGame(s_game* game);

void GameplayLoop(s_game* game);
void UpdateTransition(s_game* game);
void UpdatePlaying(s_game* game);
void UpdateMenu(s_game* game);
void UpdatePaused(s_game* game);
void RenderMenu(s_game* game);
void RenderPaused(s_game* game);
void StartCameraShake(s_game* game, float how_long, float strength);
void UpdateCameraShake(s_game* game);
void RenderGameOver(s_game* game);

void SpawnAttackers(s_game* game, int how_many);

/* Moves these to a rendering.h file */
void DrawCoordinateAxis();

#endif