#include"attacker.h"

#include"raylib.h"
#include"raymath.h"

#include"utility.h"
#include"tree.h" // s_leaf & s_tree
#include"game.h" // s_game

#include<stdlib.h>

#define SOME_THRESHOLD 0.1

void UpdateAttacker(s_tree* tree, s_attacker* attacker){
    if(!attacker->enabled || attacker->leaf_to_attack == NULL) return;

    // Is our target dead? Find another one
    if(attacker->leaf_to_attack->hp == 0){
        attacker->leaf_to_attack = GetLeaf(tree);
        // Didn't find any new alive leaf? Don't update
        if(attacker->leaf_to_attack == NULL){
            return;
        }
    }

    if(Vector2Distance(attacker->leaf_to_attack->position, attacker->position) < 0.2f){
        // The attacker has reached the leaf!
        attacker->leaf_to_attack->hp--;
        attacker->enabled = false;
        return;
    }

    // Move attacker towards leaf
    Vector2 newPosition = Vector2MoveTowards(attacker->position, attacker->leaf_to_attack->position, attacker->speed*GetFrameTime());
    //Vector2 moveDirection = Vector2Normalize(Vector2Subtract(attacker->leaf_to_attack->position, attacker->position)); // Get normalized direction
    //Vector2 newPosition = Vector2Add(attacker->position, Vector2Scale(moveDirection, attacker->speed*GetFrameTime())); // Get the new attacker position
    attacker->position = newPosition;
}

// Setup the given attacker on a new leaf
void SpawnAttacker(s_tree* tree, s_attacker* attacker){
    attacker->leaf_to_attack = GetLeaf(tree); // Grab a random leaf for the attacker
    if(attacker->leaf_to_attack == NULL) return; // Found no leaf alive so just return

    attacker->speed = GetRandomValue(40.0f, 80.0f);
    attacker->speed = 150.0f;

    attacker->position.y = GetRandomValue(0, GetScreenHeight());

    if(GetRandomFloatValue01() > 0.5f){
        attacker->position.x = -20;
    }else{
        attacker->position.x = GetScreenWidth() + 20;
    }

    attacker->enabled = true;
}

void SpawnAttackers(s_game* game, int how_many){
    int counter = 0;
    for(int i = 0; i <= MAX_ATTACKERS; i++){
        if(counter >= how_many) return; // We're done
        if(!game->attackers[i].enabled){
            counter++;
            SpawnAttacker(&game->tree, &game->attackers[i]); 
        }
    }
}

void RenderAttacker(s_attacker* attacker){
    if(!attacker->enabled) return;
    DrawCircle(attacker->position.x, attacker->position.y, 10, RED);
}