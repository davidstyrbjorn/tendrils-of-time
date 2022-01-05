#ifndef WATER_METER_H
#define WATER_METER_H

#include"raylib.h"

typedef struct s_game;

typedef struct s_water_meter {
    Vector2 origin;
    Shader shader;
    Texture texture;
    int water_level_location;
    int time_location;
} s_water_meter;

void CreateWaterMeter(s_water_meter* water_meter);
void RenderWaterMeter(struct s_game* game, s_water_meter* water_meter);

#endif