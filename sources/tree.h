#ifndef TREE_H
#define TREE_H

#include"raylib.h"

#include"constants.h"

typedef enum e_tree_status {
    HEALTHY,
    THIRSTY,
    DYING
} e_tree_status;

typedef struct s_dynamic_branch {
    // Trigonometric parameters so that every branch has a unique wind animation
    float wind_frequency;
    float wind_offset;
    float wind_amplitude;
} s_dynamic_branch;

// Data to represent a tree with branch objects for the object implementation
typedef struct s_branch {
    bool done; // Set to true if the tree has branched out
    Vector2 start;
    Vector2 end;
    float length;
    Color color;
    s_dynamic_branch dynamics;
    struct s_branch* child_a;
    struct s_branch* child_b;
    struct s_branch* parent;
} s_branch;

typedef struct s_tree {
    s_branch root; // The root branch
    s_branch* latest_added_branch;
    float base_thickness;
    float base_length;
    int indices_to_delete[400];  // utility to save indices we want to delete from vector
    Sound hurt_sound;
    e_tree_status status;
    float water_counter;
    float grow_counter;
} s_tree;

// Takes in a branch, spits out a new one going out from given branch, is recursive
// Spits out nullptr if the branch is a "leaf"
s_branch* SpawnBranch(s_branch* from, int direction);

// Draw using rlgl immediate mode
void RecursiveTreeDraw(int length, int start_length, float angle);

void CreateTree(s_tree* tree, Vector2 origin);
void UpdateTree(s_tree* tree);
void RenderTree(s_tree* tree);
void DestructTree(s_tree* tree);

void DropBranch(s_tree* tree);
void GrowTree(s_tree* tree);
void WaterTree(s_tree* tree);

bool IsBranchLeaf(s_branch* branch);

#endif 