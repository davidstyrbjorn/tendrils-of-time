#ifndef UTILITY_H
#define UTILITY_H

#include"raymath.h"

float GetRandomFloatValue(float min, float max){
    return GetRandomValue(min, max) / (float)(max - min);
}

float GetRandomFloatValue01(){
    return GetRandomFloatValue(0.0f, 1.0f);
}

#endif