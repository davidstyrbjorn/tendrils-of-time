#include"tree.h"

#include"raylib.h"
#include"rlgl.h"
#include"raymath.h"

#include"utility.h"
#include"vec.h"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MAX_RECURSION 4

#define DECAY_FACTOR .1f

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

void CreateTree(s_tree* tree, Vector2 origin) {
    tree->base_length = 100.0f;
    tree->base_thickness = 15.0f;
    tree->branch_count = 1;
    tree->iteration_levels = 8;
    tree->leaf_count = 0;
    tree->health = 1;

    // Create "dynamic" arrays
    tree->dropped_branches = vector_create();
    tree->branches = vector_create();

    // Setup first branch
    s_branch* b = vector_add_asg(&tree->branches);
    b->done = false;
    b->start = 
        Vector2Add(origin, (Vector2){GetScreenWidth()/2, GetScreenHeight()});
    b->length = tree->base_length;
    b->end = 
        Vector2Add(origin, (Vector2){GetScreenWidth()/2, GetScreenHeight()-tree->base_length});
    b = NULL;

    for(int i = 0; i < tree->iteration_levels; i++){
        int frozen_branch_count = vector_size(tree->branches);
        for(int j = 0; j < frozen_branch_count; j++){
            if(!tree->branches[j].done){
                tree->branches[j].done = true; // Mark this branch as done
                // Spawn 2 branches and insert to the end of branch list
                s_branch* b1 = vector_add_asg(&tree->branches);
                *b1 = SpawnBranch(&tree->branches[j], 1);
                b1 = NULL;
                s_branch* b2 = vector_add_asg(&tree->branches);
                *b2 = SpawnBranch(&tree->branches[j], -1);
                b2 = NULL;
            }
        }
    }

    // Do pass and find leaves
    // for(int i = 0; i < tree->branch_count; i++){
    //     if(!tree->branches[i].done){
    //         // Found leaf!
    //         tree->leaves[tree->leaf_count].position = tree->branches[i].end;
    //         tree->leaves[tree->leaf_count].hp = 3; 
    //         tree->leaves[tree->leaf_count].attached_branch = &tree->branches[i];
    //         tree->leaf_count++;
    //     }
    // }
}

void UpdateTree(s_tree* tree){
    float dt = GetFrameTime();
    tree->health -= DECAY_FACTOR*4 * dt; // Decrease tree health

    if(tree->health < 0){
        // Tree is dying! Drop a branch and increase hp some
        DropBranchAndIncreaseHealth(tree);
    }
    if(tree->health > 1.1f){
        // Tree is very healthy! Grow the tree and decreate health
        GrowTree(tree);
    }

    for(int i = 0; i < vector_size(tree->dropped_branches); i++){
        // Increase branch offset
        tree->dropped_branches[i].offset = Vector2Add(
            tree->dropped_branches[i].offset, (Vector2){0.0, 150.0f*dt});

        // Fade it out
        Color color = BROWN;
        float t = 1.0f - Clamp((GetTime() - tree->dropped_branches[i].spawn_time) / 2.5f, 0.0f, 1.0f);
        printf("%.3f\n", t);
        color.a = LinearInterpolate(0.0f, 255.0f, t);
        tree->dropped_branches[i].color = color;

        // If it has reached a clear color, remove it from the dropped branch list
    }
}

void RenderTree(s_tree* tree){
    // Branches
    for(int i = 0; i < vector_size(tree->branches); i++){
        // Branch thickness based on length of the branch, see tree->base_thickness & length
        s_branch* b = &tree->branches[i];
        float ratio = b->length / tree->base_length;
        DrawLineEx(b->start, b->end, tree->base_thickness*ratio, BROWN);

        if(!b->done){
             DrawCircle(b->end.x, b->end.y, 20, ColorAlpha(GREEN, 0.8f));
        }
    }
    // Dropped branches
    for(int i = 0; i < vector_size(tree->dropped_branches); i++){
        s_dropped_branch dropped_branch = tree->dropped_branches[i];
        float ratio = dropped_branch.branch_copy.length / tree->base_length;
        Vector2 start = Vector2Add(dropped_branch.branch_copy.start, dropped_branch.offset);
        Vector2 end = Vector2Add(dropped_branch.branch_copy.end, dropped_branch.offset);
        DrawLineEx(start, end, tree->base_thickness*ratio, dropped_branch.color);
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

void DropBranchAndIncreaseHealth(s_tree* tree) {
    tree->health += 1000;
    s_dropped_branch* dropped_branch = vector_add_asg(&tree->dropped_branches);
    // Find a branch that is done == true
    dropped_branch->branch_copy = tree->branches[vector_size(tree->branches)-1];
    dropped_branch->offset = (Vector2){0.0, 0.0};
    dropped_branch->spawn_time = GetTime();
    dropped_branch = NULL; // Stop using, the element is initalized

    vector_remove(&tree->branches, vector_size(tree->branches)-1);
}

void GrowTree(s_tree* tree) {

}

// Free memory from tree
void DestructTree(s_tree* tree){
    vector_erase(tree->branches, 0, vector_size(tree->branches));
    vector_erase(tree->dropped_branches, 0, vector_size(tree->dropped_branches)); // Erase all elements
    free(tree->branches);
    free(tree->dropped_branches);
}