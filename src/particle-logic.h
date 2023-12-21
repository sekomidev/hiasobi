#pragma once
#include "structs/Particle.h"
#include <vector>

void ClearInvisibleParticles(std::vector<Particle> &particles);
void UpdateParticlePosition(Particle &p);
void UpdateParticleState(Particle &p);