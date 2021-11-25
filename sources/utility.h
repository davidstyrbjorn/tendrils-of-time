#ifndef UTILITY_H
#define UTILITY_H

#include"raymath.h"

float GetRandomFloatValue(float min, float max){
    int a = min * 100;
    int b = max * 100;
    return GetRandomValue(a, b) / (float)(b - a);
}

float GetRandomFloatValue01(){
    return GetRandomFloatValue(0.0f, 1.0f);
}

#endif