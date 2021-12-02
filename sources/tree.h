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
} s_branch;

typedef struct s_leaf {
    Vector2 position;
    int hp;
    s_branch* attached_branch;
} s_leaf;

typedef struct s_tree {
    s_branch branches[MAX_BRANCHES];
    s_leaf leaves[MAX_LEAVES];
    int iteration_levels;
    int branch_count;
    int leaf_count;
    float base_thickness;
    float base_length;
} s_tree;

// Takes in a branch, spits out a new one going out from given branch, is recursive
// Spits out nullptr if the branch is a "leaf"
s_branch SpawnBranch(s_branch* from, int direction);

// Draw using rlgl immediate mode
void RecursiveTreeDraw(int length, int start_length, float angle);

void CreateTree(s_tree* tree);
void RenderTree(s_tree* tree);
s_leaf* GetLeaf(s_tree* tree);

#endif 