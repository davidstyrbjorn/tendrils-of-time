#ifndef TREE_H
#define TREE_H

#include"raylib.h"

#include"constants.h"

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
    s_dynamic_branch dynamics;
    struct s_branch* child_a;
    struct s_branch* child_b;
} s_branch;

typedef struct s_dropped_branch {
    s_branch branch_copy;
    Vector2 offset;
    Color color;
    double spawn_time;
} s_dropped_branch;

typedef struct s_tree {
    s_branch* branches;
    s_dropped_branch* dropped_branches;
    int iteration_levels;
    float base_thickness;
    float base_length;
    float health; // number between 0 and 1
    int indices_to_delete[100];  // utility to save indices we want to delete from vector
    float growth_chance; // number between 0 and 1
} s_tree;

// Takes in a branch, spits out a new one going out from given branch, is recursive
// Spits out nullptr if the branch is a "leaf"
s_branch SpawnBranch(s_branch* from, int direction);

// Draw using rlgl immediate mode
void RecursiveTreeDraw(int length, int start_length, float angle);

void CreateTree(s_tree* tree, Vector2 origin);
void UpdateTree(s_tree* tree);
void RenderTree(s_tree* tree);
void DestructTree(s_tree* tree);

void DropBranchAndIncreaseHealth(s_tree* tree);
void GrowTree(s_tree* tree);

bool IsBranchLeaf(s_branch* branch);

#endif 