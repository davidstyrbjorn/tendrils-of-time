#ifndef ATTACKER_H
#define ATTACKER_H

#include"raylib.h"

/*
Attackers get spawned in outside of the screen and has an assigned 
player to attack, if the attackers position reaches the leaf's position
it has succeded in the attack
*/

struct s_tree;
struct s_game;

typedef struct s_attacker {
    Vector2 position;
    int idx;
    float speed;
    bool enabled; 
    bool player_taken;
    float radius;
} s_attacker;

void UpdateAttacker(struct s_game* game, s_attacker* attacker);
s_attacker SpawnAttacker(struct s_game* game, int idx);
void RenderAttacker(s_attacker* attacker);
Vector2 GetEnemySpeed(struct s_game* game);
float GetEnemySpawnTime(struct s_game* game);

#endif