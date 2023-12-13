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
    double size;
    double lifeDecay;
    /* advanced parameters */
    double lifeAlphaMultiplier = 2;
    double startLife = 100;
    double colorAlphaAdd = 50; 
};