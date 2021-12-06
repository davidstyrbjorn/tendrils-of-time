#include"grass.h"

#include"utility.h"

void PopulateGrassField(s_grass* grass){
    grass->straw_count = 0; // Assume zero length
    float start_x = grass->origin.x - grass->horizontal_span;

    int index = 0;

    while(start_x <= grass->origin.x + grass->horizontal_span){
        // Create straw at start_x
        s_straw straw = {0};
        straw.height = grass->height + (grass->height * GetRandomFloatValue(0, grass->height_variation));
        straw.position = (Vector2){start_x, grass->origin.y};
        straw.trig_offset = GetRandomFloatValue(0, 10); // This is to make the grass "blow" out of order
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
        const float blow_factor = sin(GetTime()*grass->blow_frequency + straw.trig_offset) * grass->triangle_span*2;
        DrawTriangle(
            (Vector2){straw.position.x + blow_factor, straw.position.y - straw.height}, 
            (Vector2){straw.position.x-grass->triangle_span, straw.position.y}, 
            (Vector2){straw.position.x+grass->triangle_span, straw.position.y}, 
            GREEN
        );
    }
}