#include"water_meter.h"

#include"game.h"
#include"external/glad.h"

void CreateWaterMeter(s_water_meter* water_meter){
    water_meter->shader = LoadShader(0, TextFormat(ASSETS_PATH"water_meter.glsl", 330));
    const int width = 400;
    const int height = 40;
    Image imBlank = GenImageColor(width, height, BLANK);
    water_meter->texture = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);
    water_meter->water_level_location = glGetUniformLocation(water_meter->shader.id, "waterLevel");
    water_meter->time_location = glGetUniformLocation(water_meter->shader.id, "time");
}

void RenderWaterMeter(s_game* game, s_water_meter* water_meter){
    // Render texture with water level shader
    BeginShaderMode(water_meter->shader);
        DrawTexture(water_meter->texture, 10, 10, WHITE);
    EndShaderMode();
}