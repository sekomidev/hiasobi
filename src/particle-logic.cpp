/* Deletes particles that are not visible from memory. */
#include "rand-utils.h"
#include "structs/Particle.h"
#include <algorithm>
#include <vector>

const int W_MAX_OUT_OF_BOUNDS_OFFSET = 1000;

void ClearInvisibleParticles(std::vector<Particle> &particles)
{
    particles.erase(std::remove_if(begin(particles), end(particles),
                                   [](Particle p) {
                                       return p.pos.y < -W_MAX_OUT_OF_BOUNDS_OFFSET ||
                                              p.pos.y > GetScreenHeight() + W_MAX_OUT_OF_BOUNDS_OFFSET ||
                                              p.pos.x < -W_MAX_OUT_OF_BOUNDS_OFFSET ||
                                              p.pos.x > GetScreenWidth() + W_MAX_OUT_OF_BOUNDS_OFFSET || p.life < 0;
                                   }),
                    end(particles));
}

/*
    Updates only the physical position of the particle.
    Does not modify particle inertia or speed.
*/
void UpdateParticlePosition(Particle &p)
{
    auto randDeltaX = RandDouble(p.type->minRandMove.x, p.type->maxRandMove.x);
    auto randDeltaY = RandDouble(p.type->minRandMove.y, p.type->maxRandMove.y);
    p.pos.x += (p.inertia.x + randDeltaX) * 60 * GetFrameTime();
    p.pos.y -= (p.inertia.y + randDeltaY) * 60 * GetFrameTime();
}

/* Updates the state of the particle, such as it's life, speed or inertia. */
void UpdateParticleState(Particle &p)
{
    p.life -= p.type->lifeDecay * 60 * GetFrameTime();
    p.color.a = p.type->colorAlphaAdd + p.life * p.type->lifeAlphaMultiplier;

    auto inertDeltaX = RandDouble(p.type->minInertiaAdd.x, p.type->maxInertiaAdd.x) * 60 * GetFrameTime();
    if (p.inertia.x + inertDeltaX >= p.type->minInertia.x && p.inertia.x + inertDeltaX <= p.type->maxInertia.x) {
        p.inertia.x += inertDeltaX;
    }

    auto inertDeltaY = RandDouble(p.type->minInertiaAdd.y, p.type->maxInertiaAdd.y) * 60 * GetFrameTime();
    if (p.inertia.y + inertDeltaY >= p.type->minInertia.y && p.inertia.y + inertDeltaY <= p.type->maxInertia.y) {
        p.inertia.y += inertDeltaY;
    }

    UpdateParticlePosition(p);
}