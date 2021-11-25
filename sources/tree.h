#ifndef TREE_H
#define TREE_H

#include"raylib.h"

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

// Takes in a branch, spits out a new one going out from given branch, is recursive
// Spits out nullptr if the branch is a "leaf"
s_branch SpawnBranch(s_branch* from, int direction);

// Draw using rlgl immediate mode
void RecursiveTreeDraw(int length, int start_length, float angle);

#endif 