#include"tree.h"

#include"raylib.h"
#include"rlgl.h"
#include"raymath.h"

#include"utility.h"
#include"vec.h"
#include"cvector.h"
#include"game.h"
#include"seed.h"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define MAX_RECURSION 4

#define DECAY_FACTOR .1f

Vector2 rotate_point(float cx, float cy, float angle, Vector2 p){
    return (Vector2){cos(angle) * (p.x - cx) - sin(angle) * (p.y - cy) + cx,
        sin(angle) * (p.x - cx) + cos(angle) * (p.y - cy) + cy};
}

char* tree_status_to_string(s_tree* tree){
    if(tree->status == HEALTHY)
        return "HEALTHY";
    else if(tree->status == THIRSTY)
        return "THIRSTY";
    else if(tree->status == DYING)
        return "DYING";
    return "UNKOWN STATUS";
}

s_branch* SpawnBranch(s_branch* from, int direction){
    Vector2 start = from->end;
    float x = 0.9f;
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
    s_branch* new_branch = (s_branch*)malloc(sizeof(s_branch));
    new_branch->start = start;
    new_branch->end = end;
    new_branch->length = length;
    new_branch->done = false;
    new_branch->dynamics = dynamic_branch;
    new_branch->color = BROWN;
    new_branch->child_a = NULL;
    new_branch->child_b = NULL;
    new_branch->parent = from;
    new_branch->seed_counter = 0;
    new_branch->drop_seed_at = GetRandomValue(10, 20);
    new_branch->growing = true;
    new_branch->t = 0.0f;           

    return new_branch;
}

void RecursiveGenerate(s_branch* branch, int depth, s_tree* tree){
    if(depth > 0) return; // Break condition

    if(depth > tree->max_depth){
        tree->max_depth = depth;
    }                                   

    if(branch->child_b == NULL){
        // Generate child b
        branch->child_b = SpawnBranch(branch, 1);
        tree->latest_added_branch = branch->child_b;
    }
    if(branch->child_a == NULL){
        // Generate child a
        branch->child_a = SpawnBranch(branch, -1);
        tree->latest_added_branch = branch->child_b;
    }

    // Continue down and increase depth
    RecursiveGenerate(branch->child_a, depth+1, tree);
    RecursiveGenerate(branch->child_b, depth+1, tree);
}

void CreateTree(s_tree* tree, Vector2 origin) {
    tree->base_length = 90.0f;
    tree->base_thickness = 15.0f;
    tree->water_counter = 100;
    tree->status = HEALTHY;
    tree->dying_counter = 0;
    tree->grow_counter = 0;

    // Create root
    tree->root = (s_branch){0};
    tree->root.child_a = NULL;
    tree->root.child_b = NULL;
    tree->root.done = false;
    tree->root.start = 
        Vector2Add(origin, (Vector2){GetScreenWidth()/2, GetScreenHeight()});
    tree->root.length = tree->base_length;
    tree->root.end = 
        Vector2Add(origin, (Vector2){GetScreenWidth()/2, GetScreenHeight()-tree->base_length});

    tree->max_depth = 0;
    // Generate a bunch of levels
    RecursiveGenerate(&tree->root, 0, tree);

    // Load sound
    tree->hurt_sound = LoadSound(ASSETS_PATH"tree_hurt.wav");

    //for(int i = 0; i < MAX_SEED_COUNT; i++) { tree->seeds[i].enabled = false; }
}

void UpdateTree(s_tree* tree, s_game* game){
    float dt = GetFrameTime();
    // Decrement and clamp water counter
    tree->water_counter -= 6.0f * dt;
    if(tree->water_counter <= 0){
        tree->water_counter = 0;
    }
    // Update status
    if(tree->water_counter <= 30) tree->status = DYING;
    else if(tree->water_counter <= 75) tree->status = THIRSTY;
    else tree->status = HEALTHY;

    // Update depending on current tree status
    if(tree->status == HEALTHY){
        tree->grow_counter += 1.0f * dt;
        if(tree->grow_counter >= 8){
            GrowTree(tree);
            tree->grow_counter = 0;
        }
    }
    if(tree->status == DYING || tree->status == THIRSTY){
        tree->dying_counter += 1.0f * dt;

        if(tree->dying_counter >= 2 && tree->status == DYING){
            DropBranch(tree);
            tree->dying_counter = 0;
            if(tree->root.child_a == NULL && tree->root.child_b == NULL){
                // Tell game its game over
                game->game_state = GAME_OVER;
                StopMusicStream(game->bg_music);
                PlayMusicStream(game->game_over_music);
            }
        }
    }


    // Update all our seeds
    for(int i = 0; i < MAX_SEED_COUNT; i++){
        if(tree->seeds[i].enabled) UpdateSeed(game, &tree->seeds[i]);
    }
}

void RenderTreeRecursive(s_branch* branch, int depth, s_tree* tree, int* leaf_count){
    if(branch == NULL) return; // Break condition

    // Check max depth
    if(depth > tree->max_depth){
        tree->max_depth = depth;
    }

    // Increase t (growing parameter)
    if(branch->t < 1.0f){
        branch->t += 0.5f * GetFrameTime();
        branch->growing = true;
    }else{
        branch->growing = false;
    }
    Vector2 dir = Vector2Scale(Vector2Subtract(branch->end, branch->start), branch->t);
    Vector2 end = Vector2Add(branch->start, dir);

    // Draw line
    float ratio = 1 - (depth / (float)tree->max_depth);
    DrawLineEx(branch->start, end, tree->base_thickness*(ratio+0.5f), branch->color);

    // do we have a leaf
    if(IsBranchLeaf(branch) && !branch->growing){
        tree->leafs[*leaf_count].position = branch->end;
        *leaf_count = *leaf_count+1;
    }

    // Update seed status for branch, only do it for non-root branches (depth > 0)
    if(depth > 0){
        branch->seed_counter += 1.0f * GetFrameTime();
        if(branch->seed_counter > branch->drop_seed_at){
            branch->seed_counter = 0.0f;
            CreateSeed(tree, branch->end);
        }
    }

    // Traverse
    RenderTreeRecursive(branch->child_a, depth+1, tree, leaf_count);
    RenderTreeRecursive(branch->child_b, depth+1, tree, leaf_count);
}

void RenderTree(s_tree* tree){
    // Draw root
    s_branch* root = &tree->root;
    int leaf_count = 0;
    DrawLineEx(root->start, root->end, tree->base_thickness, BROWN);
    RenderTreeRecursive(root, 0, tree, &leaf_count);
    // Render the leafs now
    for(int i = 0; i < leaf_count; i++){
        DrawCircleV(tree->leafs[i].position, 20, ColorAlpha(GREEN, 0.8f));
    }
    // Render seeds
    for(int i = 0; i < MAX_SEED_COUNT; i++){
        if(tree->seeds[i].enabled) RenderSeed(&tree->seeds[i]);
    }

    // Render text
    char status_lit[20] = "Tree Status: ";
    strcat(status_lit, tree_status_to_string(tree));
    DrawText(status_lit, 10, 60, 32, WHITE);
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

void GrowTreeR(s_branch* branch, int depth){
    if(branch == NULL) return; // Break condition
    
    // Add branches if non exists on this branch already
    if(branch->child_a == NULL && GetRandomFloatValue01() > 0.5f){
        branch->child_a = SpawnBranch(branch, -1);
        return;
    }
    if(branch->child_b == NULL && GetRandomFloatValue01() > 0.5f){
        branch->child_b = SpawnBranch(branch, 1);
        return;
    }
    
    GrowTreeR(branch->child_a, depth+1);
    GrowTreeR(branch->child_b, depth+1);
}

void GrowTree(s_tree* tree) {
    GrowTreeR(&tree->root, 0);
}

void FindDeepestBranch(s_branch* curr, int level, int* deepest_level, s_branch** res){
    if(curr != NULL){
        FindDeepestBranch(curr->child_a, ++level, deepest_level, res);
        if(level > *deepest_level){
            *deepest_level = level;
            *res = curr;
        }
        FindDeepestBranch(curr->child_b, level, deepest_level, res);
    }
}

void DropBranch(s_tree* tree){
    if(tree->root.child_a == NULL && tree->root.child_b == NULL) return; // Chill if this is the case
    s_branch* deepest_branch = NULL;
    int deepest_level = -1;
    FindDeepestBranch(&tree->root, 0, &deepest_level, &deepest_branch);

    // Invalidate pointer adresses
    if(deepest_branch->parent->child_a == deepest_branch){
        deepest_branch->parent->child_a = NULL;
    }else{
        deepest_branch->parent->child_b = NULL;
    }
    free(deepest_branch); deepest_branch = NULL;
}

void RecursivelyFreeTree(s_branch* branch){
    if(branch == NULL) return;

    s_branch* a = branch->child_a;
    s_branch* b = branch->child_b;
    free(branch);

    RecursivelyFreeTree(branch->child_a);
    RecursivelyFreeTree(branch->child_b);
}

// Free memory from tree
void DestructTree(s_tree* tree){
    UnloadSound(tree->hurt_sound);
    RecursivelyFreeTree(&tree->root);
}

bool IsBranchLeaf(s_branch* branch){
    return (branch->child_a == NULL || branch->child_b == NULL);
}

void WaterTree(s_tree* tree){
    // Increase water counter, clamp to 3
    tree->water_counter+=25;
    if(tree->water_counter >= 100) tree->water_counter = 100;
}