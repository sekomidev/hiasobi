#pragma once
#include "ParticleType.h"

struct Particle
{
    ParticleType *type;
    Vector2 pos;
    Vector2 inertia;
    Color color;
    float life;
    float size;
};