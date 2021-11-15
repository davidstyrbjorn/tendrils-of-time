#include"tree.h"

#include<stdio.h>

#define MAX_RECURSION 4

s_branch* SpawnBranch(s_branch* branch){
    // If we're at the end of a branch
    if(branch->recursive_lvl >= MAX_RECURSION)
        return NULL;
    
    
}