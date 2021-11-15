#include"tree.h"

#include<stdio.h>
#include<stdlib.h>

#include"raymath.h"

#define MAX_RECURSION 4

s_branch* SpawnBranch(s_branch* branch){
    // If we're at the end of a branch
    if(branch->recursive_lvl >= MAX_RECURSION)
        return NULL; // Break recursion

    // Allocate new branch 
    s_branch* new_branch = malloc(sizeof(s_branch));

    new_branch->norm_direction = (Vector2){0.0, 1.0};  // Set the direction of the branch
    new_branch->object.position = Vector2Add(branch->object.position, Vector2Scale(branch->norm_direction, branch->length)); // Set new origin
    int lvl = ++new_branch->recursive_lvl; // Increase recursive level
    new_branch->length = branch->length*((MAX_RECURSION - lvl)/(float)MAX_RECURSION); // Length of the new branch based on the last one (linear decline)
    new_branch->object.color = branch->object.color; // Copy the object

    return SpawnBranch(new_branch); // Recursive call, will return NULL at some point
} 