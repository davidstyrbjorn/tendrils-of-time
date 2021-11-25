#include"tree.h"

#include"raylib.h"
#include"rlgl.h"
#include"raymath.h"

#include"utility.h"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"raymath.h"

#define MAX_RECURSION 4

Vector2 rotate_point(float cx, float cy, float angle, Vector2 p){

     return (Vector2){cos(angle) * (p.x - cx) - sin(angle) * (p.y - cy) + cx,
                  sin(angle) * (p.x - cx) + cos(angle) * (p.y - cy) + cy};
}

s_branch SpawnBranch(s_branch* from, int direction){
    Vector2 start = from->end;
    float x = GetRandomValue(60, 95) / 100.0f;
    float length = from->length*x;
    Vector2 end = (Vector2){
        from->end.x, 
        from->end.y-length
    };

    // Rotate end around start
    float angle = (30 + GetRandomValue(0, 40)) * DEG2RAD;
    end = rotate_point(start.x, start.y, angle*direction, end);

    // Generate some dynamic data for the branch
    s_dynamic_branch dynamic_branch;
    dynamic_branch.wind_amplitude = 10.0f;
    dynamic_branch.wind_frequency = GetRandomFloatValue01()*3.0f;
    dynamic_branch.wind_offset = GetRandomValue(0, 100);

    // Create the object
    s_branch new_branch;
    new_branch.start = start;
    new_branch.end = end;
    new_branch.length = length;
    new_branch.done = false;
    new_branch.dynamics = dynamic_branch;

    return new_branch;
}

void CreateTree(s_tree* tree) {
    tree->base_length = 100.0f;
    tree->base_thickness = 15.0f;
    tree->branch_count = 1;
    tree->iteration_levels = 8;
    tree->leaf_count = 0;

    // Setup first branch
    tree->branches[0].done = false;
    tree->branches[0].start = (Vector2){GetScreenWidth()/2, GetScreenHeight()};
    tree->branches[0].length = tree->base_length;
    tree->branches[0].end = (Vector2){GetScreenWidth()/2, GetScreenHeight()-tree->base_length};

    for(int i = 0; i < tree->iteration_levels; i++){
        int frozen_branch_count = tree->branch_count;
        for(int j = 0; j < frozen_branch_count; j++){
            if(!tree->branches[j].done){
                // Spawn 2 branches and insert to the end of branch list
                tree->branches[j].done = true;
                tree->branches[tree->branch_count] = SpawnBranch(&tree->branches[j], 1);
                tree->branch_count++;
                tree->branches[tree->branch_count] = SpawnBranch(&tree->branches[j], -1);
                tree->branch_count++;
            }
        }
    }

    // Do pass and find leaves
    for(int i = 0; i < tree->branch_count; i++){
        if(!tree->branches[i].done){
            // Found leaf!
            tree->leaves[tree->leaf_count++].position = tree->branches[i].end;
        }
    }
}

void RenderTree(s_tree* tree){
    for(int i = 0; i < tree->branch_count; i++){
        // Branch thickness based on length of the branch, see tree->base_thickness & length
        s_branch b = tree->branches[i];
        float ratio = b.length / tree->base_length;
        DrawLineEx(b.start, b.end, tree->base_thickness*ratio, BROWN);
        // This is true if we're on a branch that has no children
        if(!b.done){
            Vector2 wind = {0};
            float temp = b.dynamics.wind_offset + b.dynamics.wind_frequency*GetTime();
            wind.x = cos(temp) * b.dynamics.wind_amplitude;
            //wind.y = sin(temp) * b.dynamics.wind_amplitude;
            DrawCircle(b.end.x + wind.x, b.end.y + wind.y, 20, ColorAlpha(GREEN, 0.8f));
        }
    }
}

void RecursiveTreeDraw(int length, int start_length, float angle){
    const float branch_decay = 0.67;
    const float min_length = 12;
    const float thickness_max = 30;

    // Calc thickness of line based on length
    float thickness = thickness_max * (length/(float)start_length);
    // Draw line
    DrawLineEx((Vector2){0.0, 0.0}, (Vector2){0.0, -length}, thickness, BROWN);
    rlTranslatef(0, -length, 0);
    if(length > min_length){
        
        rlPushMatrix();
        rlRotatef(angle, 0, 0, 1);
        RecursiveTreeDraw(length * branch_decay, start_length, angle);
        rlPopMatrix();

        rlPushMatrix();
        rlRotatef(-angle, 0, 0, 1);
        RecursiveTreeDraw(length * branch_decay, start_length, angle);
        rlPopMatrix();
    }
}

s_leaf* GetLeaf(s_tree* tree){
    return &tree->leaves[0]; // Just return the first leaf
}