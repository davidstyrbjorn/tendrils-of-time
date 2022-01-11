#ifndef SEED_H
#define SEED_H

#include"raylib.h"

typedef struct s_game;
typedef struct s_tree;

typedef struct s_seed {
    Vector2 position;
    bool enabled;
} s_seed;

void CreateSeed(struct s_tree* tree, Vector2 position);
void RenderSeed(s_seed* seed);
void UpdateSeed(struct s_game* game, s_seed* seed);

#endif