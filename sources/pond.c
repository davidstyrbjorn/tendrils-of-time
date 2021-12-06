#include"pond.h"

#include"external/glad.h"

void CreatePond(s_pond* pond) {
    pond->shader = LoadShader(0, TextFormat(ASSETS_PATH"water.glsl", 330));
    Image imBlank = GenImageColor(pond->width, 300, BLANK);
    pond->water_texture = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);
    pond->time_location = glGetUniformLocation(pond->shader.id, "time");
}

void RenderPond(s_pond* pond) {
    // Render the texture with the water shader
    BeginShaderMode(pond->shader);
        DrawTexture(pond->water_texture, pond->origin.x, pond->origin.y, WHITE);
    EndShaderMode();
}

void UnloadPond(s_pond* pond) {
    UnloadTexture(pond->water_texture);
    UnloadShader(pond->shader);
}