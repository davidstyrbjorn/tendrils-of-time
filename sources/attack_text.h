#ifndef ATTACK_TEXT_H
#define ATTACK_TEXT_H

#include"raylib.h"

typedef struct s_game;

typedef struct s_attack_text {
    Vector2 position;
    const char* text;
    Color color;
    float randomized_x_speed;
    float t; // t: 0 -> 1
    bool enabled;
} s_attack_text;

void CreateAttackText(struct s_game* game, Vector2 position, const char* text, Color color); // Factory function
void UpdateAttackText(s_attack_text* attack_text);
void RenderAttackText(s_attack_text* attack_text);

#endif
