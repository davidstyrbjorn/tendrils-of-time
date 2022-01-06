#include"tree.h"

#include"raylib.h"
#include"rlgl.h"
#include"raymath.h"

#include"utility.h"
#include"vec.h"
#include"cvector.h"

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
    new_branch->child_a = NULL;
    new_branch->child_b = NULL;

    return new_branch;
}

void RecursiveGenerate(s_branch* branch, int depth){
    if(depth > 2) return; // Break condition

    if(branch->child_a == NULL){
        // Generate child a
        branch->child_a = SpawnBranch(branch, -1);
    }
    if(branch->child_b == NULL){
        // Generate child b
        branch->child_b = SpawnBranch(branch, 1);
    }

    // Continue down and increase depth
    RecursiveGenerate(branch->child_a, depth+1);
    RecursiveGenerate(branch->child_b, depth+1);
}

void CreateTree(s_tree* tree, Vector2 origin) {
    tree->base_length = 90.0f;
    tree->base_thickness = 15.0f;
    tree->water_counter = 100;
    tree->status = HEALTHY;

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

    // Generate a bunch of levels
    RecursiveGenerate(&tree->root, 0);

    // Load sound
    tree->hurt_sound = LoadSound(ASSETS_PATH"tree_hurt.wav");

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
    // Update status
    tree->water_counter -= 6.0f * dt;
    if(tree->water_counter <= 0){
        tree->water_counter = 0;
    }
    if(tree->water_counter <= 25) tree->status = DYING;
    else if(tree->water_counter <= 70) tree->status = THIRSTY;
    else tree->status = HEALTHY;

    if(tree->status == HEALTHY){
        tree->grow_counter += 1.0f * dt;
        if(tree->grow_counter >= 5){
            GrowTree(tree);
            tree->grow_counter = 0;
        }
    }
    if(tree->status == DYING){
        tree->grow_counter += 1.0f * dt;
        if(tree->grow_counter >= 4){
            DropBranch(tree);
            tree->grow_counter = 0;
        }
    }
}

void RenderTreeRecursive(s_branch* branch, s_tree* tree){
    if(branch == NULL) return; // Break condition

    // Draw line
    DrawLineEx(branch->start, branch->end, tree->base_thickness, BROWN);

    // Traverse
    RenderTreeRecursive(branch->child_a, tree);
    RenderTreeRecursive(branch->child_b, tree);
}

void RenderTree(s_tree* tree){
    // Draw root
    s_branch* root = &tree->root;
    DrawLineEx(root->start, root->end, tree->base_thickness, BROWN);
    RenderTreeRecursive(root, tree);

    // Render text
    char status_lit[20] = "Tree Status: ";
    strcat(status_lit, tree_status_to_string(tree));
    DrawText(status_lit, 0, 50, 32, WHITE);
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
    // Calculate new branch length, depth is a number of how far we are in
    // grow proprtionally to depth^2 so branches further in grow more
    Vector2 line = Vector2Subtract(branch->end, branch->start);
    Vector2 direction = Vector2Scale(Vector2Normalize(line), depth*depth);
    line = Vector2Add(line, direction);
    branch->end = Vector2Add(branch->start, line); // new end
    // If we have children, tell them about their new start
    if(branch->child_a != NULL)
        branch->child_a->start = branch->end;
    if(branch->child_b != NULL)
        branch->child_b->start = branch->end;
    // Since tree is basically a binary-tree data structure, we use recursion iteration
    GrowTreeR(branch->child_a, depth+1);
    GrowTreeR(branch->child_b, depth+1);
}

void GrowTree(s_tree* tree) {
    // GrowTreeR(current, 0);
}

void DropBranch(s_tree* tree){

}

// Free memory from tree
void DestructTree(s_tree* tree){
    
}

bool IsBranchLeaf(s_branch* branch){
    return (branch->child_a == NULL && branch->child_b == NULL);
}

void WaterTree(s_tree* tree){
    // Increase water counter, clamp to 3
    tree->water_counter+=25;
    if(tree->water_counter >= 100) tree->water_counter = 100;
}