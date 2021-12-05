#include"grass.h"

#include"utility.h"

void PopulateGrassField(s_grass* grass){
    grass->straw_count = 0; // Assume zero length
    float start_x = grass->origin.x - grass->horizontal_span;

    int index = 0;

    while(start_x <= grass->origin.x + grass->horizontal_span){
        // Create straw at start_x
        s_straw straw = {0};
        straw.height = grass->height + (grass->height);
        straw.position = (Vector2){start_x, grass->origin.y - straw.height};
        grass->straws[grass->straw_count] = straw; // Insert into array
        // Progress forward at random step length
        start_x += grass->density * GetRandomFloatValue01();
        grass->straw_count++;
    }
}

void RenderGrass(s_grass* grass){
    for(int i = 0; i < grass->straw_count; i++) {
        s_straw straw = grass->straws[i];
        // Render a rectangle
        DrawRectangle(straw.position.x, straw.position.y, 5, straw.height, GREEN);
    }
}