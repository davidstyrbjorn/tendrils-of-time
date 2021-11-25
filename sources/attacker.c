#include"attacker.h"

#include"raylib.h"
#include"raymath.h"

#include"utility.h"
#include"tree.h" // s_leaf & s_tree

#define SOME_THRESHOLD 1e-2

void UpdateAttacker(s_attacker* attacker){
    if(Vector2Distance(attacker->leaf_to_attack->position, attacker->position) < SOME_THRESHOLD){
        // The attacker has reached the leaf!
        return;
    }

    // Move attacker towards leaf
    Vector2 moveDirection = Vector2Normalize(Vector2Subtract(attacker->leaf_to_attack->position, attacker->position)); // Get normalized direction
    Vector2 newPosition = Vector2Add(attacker->position, Vector2Scale(moveDirection, attacker->speed*GetFrameTime())); // Get the new attacker position
    attacker->position = newPosition;
}

// Setup the given attacker on a new leaf
void SpawnAttacker(s_tree* tree, s_attacker* attacker){
    attacker->leaf_to_attack = GetLeaf(tree); // Grab a random leaf for the attacker

    attacker->speed = GetRandomValue(40.0f, 80.0f);

    attacker->position.y = GetRandomValue(0, GetScreenHeight());

    if(GetRandomFloatValue01() > 0.5f){
        attacker->position.x = -20;
    }else{
        attacker->position.x = GetScreenWidth() + 20;
    }

    attacker->enabled = true;
}

void RenderAttacker(s_attacker* attacker){
    DrawCircle(attacker->position.x, attacker->position.y, 10, RED);
}