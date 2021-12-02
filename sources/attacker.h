#ifndef ATTACKER_H
#define ATTACKER_H

#include"raylib.h"

/*
Attackers get spawned in outside of the screen and has an assigned 
leaf to attack, if the attackers position reaches the leaf's position
it has succeded in the attack
*/

struct s_leaf; // leaf_to_attack forward declaration
struct s_tree;
struct s_game;

typedef struct s_attacker {
    Vector2 position;
    float speed;
    struct s_leaf* leaf_to_attack;
    bool enabled; 
} s_attacker;

void UpdateAttacker(struct s_tree* tree, s_attacker* attacker);
void SpawnAttackers(struct s_game* game, int how_many);
void SpawnAttacker(struct s_tree* tree, s_attacker* attacker);
void RenderAttacker(s_attacker* attacker);

#endif