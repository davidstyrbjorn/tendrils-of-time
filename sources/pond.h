#ifndef POND_H
#define POND_H

#include"raylib.h"

typedef struct s_pond {
    Vector2 origin;
    float width;
    Shader shader;
    Texture water_texture;
    int time_location;
} s_pond;

void CreatePond(s_pond* pond);
void RenderPond(s_pond* pond);
void UnloadPond(s_pond* pond);

#endif