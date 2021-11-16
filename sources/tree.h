#ifndef TREE_H
#define TREE_H

#include"object.h"
#include"raylib.h"

// Data to represent a tree with branch objects for the object implementation

typedef struct s_branch {
    s_object object;    
    struct s_branch* branch; // Linked list of branches
    int recursive_lvl;
    int branch_width;
    int branch_length;
    float angle;
    Vector2 origin;
} s_branch;

// Takes in a branch, spits out a new one depending on some branch paramters, is recursive
// Spits out nullptr if the branch is a "leaf"
s_branch* SpawnBranch(s_branch* branch);

// Draw using rlgl immediate mode
void RecursiveTreeDraw(int length, int start_length, float angle);

#endif 