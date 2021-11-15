#ifndef OBJECT_H
#define OBJECT_H

#include"raylib.h"

// I'm thinking this is a structure which holds basic data for all objects
typedef struct s_object {
    Vector2 position;
    Color color;
} s_object;

#endif