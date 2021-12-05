#ifndef GRASS_H
#define GRASS_H

#include"raylib.h"

/*
    Data for a field of grass + its straws
*/

#define MAX_STRAW_COUNT 256

typedef struct s_straw {
    Vector2 position;
    float height;
} s_straw;

typedef struct s_grass {
    Vector2 origin;
    float horizontal_span;
    float height;
    float height_variation; // In % of height
    float density;
    
    int straw_count;
    s_straw straws[MAX_STRAW_COUNT];
} s_grass;


void PopulateGrassField(s_grass* grass); // Populates straws data field for a specified grass field
void RenderGrass(s_grass* grass);

#endif