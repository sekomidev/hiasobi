#include "raylib.h"
#include "rand.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

// TODO: try to use a linked list to store the particles

int         A_TARGET_FPS = 60;
const int   W_MAX_PARTICLES = 100'000;
const int   W_MAX_OUT_OF_BOUNDS_OFFSET = 100;

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

struct Particle
{
    ParticleType *type;
    Vector2 pos;
    Vector2 inertia;
    Color color;
    float intensity;
    float size;
};

struct Brush
{
    ParticleType *particleType;
    int amount;
    int spread;
};

void CleanParticles(std::vector<Particle> &particles)
{
    for (int i = 0; i < particles.size(); i++) 
    {
        if (particles[i].pos.y < -W_MAX_OUT_OF_BOUNDS_OFFSET 
         || particles[i].pos.y >  GetScreenHeight() + W_MAX_OUT_OF_BOUNDS_OFFSET 
         || particles[i].pos.x < -W_MAX_OUT_OF_BOUNDS_OFFSET
         || particles[i].pos.x >  GetScreenWidth() + W_MAX_OUT_OF_BOUNDS_OFFSET
         || particles[i].intensity < 0) 
        {
            particles.erase(particles.begin() + i);
        }
    }
}

void UpdateParticlePosition(std::vector<Particle> &particles, const int &i) 
{
    auto p = &particles[i];
    particles[i].pos.x += (particles[i].inertia.x + RandFloat(p->type->minRandMove.x, p->type->maxRandMove.x)) * GetFrameTime() * 100;
    particles[i].pos.y -= (particles[i].inertia.y + RandFloat(p->type->minRandMove.y, p->type->maxRandMove.y)) * GetFrameTime() * 100;
}

void UpdateParticleState(std::vector<Particle> &particles, const int &i) {
    auto p = &particles[i];
    particles[i].intensity -= p->type->intensityDecay;
    particles[i].color.a = 50 + particles[i].intensity * 2;

    auto inertDeltaX = RandFloat(p->type->minInertiaAdd.x, p->type->maxInertiaAdd.x);
    if (particles[i].inertia.x + inertDeltaX > p->type->minInertia.x
     && particles[i].inertia.x + inertDeltaX < p->type->maxInertia.x ) 
    {
        particles[i].inertia.x += inertDeltaX;
    }

    auto inertDeltaY = RandFloat(p->type->minInertiaAdd.y, p->type->maxInertiaAdd.y);
     if (particles[i].inertia.y + inertDeltaY > p->type->minInertia.y
     && particles[i].inertia.y + inertDeltaY < p->type->maxInertia.y) 
    {
        particles[i].inertia.y += inertDeltaY;
    }

    UpdateParticlePosition(particles, i);
}

void UpdateParticles(std::vector<Particle> &particles) 
{
    CleanParticles(particles);

    for (int i = 0; i < particles.size(); i++) 
    {
        UpdateParticleState(particles, i);
    }
}

void DrawParticles(const std::vector<Particle> &particles)
{
    for (int i = 0; i < particles.size(); i++) 
    {
        auto p = &particles[i];
        DrawRectangle(p->pos.x, p->pos.y, p->size, p->size, p->color);
    }
}

void AddParticlesAtPos(std::vector<Particle> &particles, ParticleType &type, const Vector2 pos, const int amount, const int spread)
{
    if (particles.size() > W_MAX_PARTICLES) 
    {
        return;
    }
    
    for (int i = 0; i < amount; i++) 
    {
        Vector2 randOffset = RandPointInCircle(spread);
        particles.push_back({&type, {pos.x + randOffset.x, pos.y + randOffset.y}, {0, 0}, type.color, 100, type.size});
    }
}

void AddParticlesAtMousePos(std::vector<Particle> &particles, ParticleType &type, const int amount, const int spread)
{
    Vector2 mousePos = GetMousePosition();
    AddParticlesAtPos(particles, type, mousePos, amount, spread);
}

void AppInit()
{
	InitWindow(1280, 800, "hiasobi (camellia reference!!!)");
    SetWindowIcon(LoadImage("resources/common/icon.png"));
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(A_TARGET_FPS);
    SetTraceLogLevel(LOG_DEBUG);
	SetExitKey(KEY_ESCAPE);
}

void DrawDebugInfo(std::vector<Particle> &particles)
{
    DrawFPS(1200, 20);
    DrawText(std::to_string(particles.size()).c_str(), 1200, 40, 24, GRAY);
}

int main() 
{
    AppInit();
    std::vector<Particle> particles;
    ParticleType fire
    {
        .minInertia = {-1, -1},
        .maxInertia = {1, 2.5},
        .minInertiaAdd = {-0.2, -0.1},
        .maxInertiaAdd = {0.2, 0.15},
        .minRandMove = {-2, -0.8},
        .maxRandMove = {2, 2},
        .color = RED,
        .intensityDecay = 1.5,
        .size = 4
    };
    ParticleType water
    {
        .minInertia = {-1, -4},
        .maxInertia = {1, 0},
        .minInertiaAdd = {-0.5, -2},
        .maxInertiaAdd = {0.5, -1},
        .minRandMove = {-0.5, 0},
        .maxRandMove = {0.5, 0},
        .color = BLUE,
        .intensityDecay = 0.5,
        .size = 4
    };
    Brush fireBrush =
    {
        .particleType = &fire,
        .amount = 5,
        .spread = 16
    };
    Brush waterBrush =
    {
        .particleType = &water,
        .amount = 16,
        .spread = 32
    };
    Brush currentBrush = fireBrush;

	// game loop
	while (!WindowShouldClose()) 
	{
        UpdateParticles(particles);

        // TODO: get all of this inside some function
        switch (GetKeyPressed()) 
        {
            case KEY_W:
                currentBrush = waterBrush;
                break;
            case KEY_F:
                currentBrush = fireBrush;
                break;
            // TODO: fix window not resizing properly
            case KEY_F11:
            {
                int display = GetCurrentMonitor();
                std::cout << display << " " << GetMonitorWidth(display) << std::endl;
                ToggleFullscreen();
                
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
                break;
            }
            case KEY_C:
                fire.color = RandColorRgb(0, 0, 0, 255, 255, 255);
                break;
            case KEY_MINUS:
                fire.size -= 1;
                break;
            case KEY_EQUAL:
                fire.size += 1;
                break;
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_X)) 
        {
            AddParticlesAtMousePos(particles, *currentBrush.particleType, currentBrush.amount, currentBrush.spread);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawParticles(particles);
        DrawDebugInfo(particles);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}