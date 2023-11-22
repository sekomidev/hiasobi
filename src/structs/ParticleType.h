#pragma once
#include "raylib.h"

struct ParticleType
{
    /* movement parameters */
    Vector2 minInertia;
    Vector2 maxInertia;
    Vector2 minInertiaAdd;
    Vector2 maxInertiaAdd;
    Vector2 minRandMove;
    Vector2 maxRandMove;
    /* visual parameters */
    Color color;
    float size;
    float lifeDecay;
    /* advanced parameters */
    float lifeAlphaMultiplier = 2;
    float startLife = 100;
    float colorAlphaAdd = 50; 
};