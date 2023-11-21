#pragma once
#include "raylib.h"

struct ParticleType
{
    Vector2 minInertia;
    Vector2 maxInertia;
    Vector2 minInertiaAdd;
    Vector2 maxInertiaAdd;
    Vector2 minRandMove;
    Vector2 maxRandMove;
    Color color;
    float intensityDecay;
    float size;
};