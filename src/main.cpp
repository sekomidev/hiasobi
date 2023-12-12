#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "file-saveload.h"
#include "rand-utils.h"
#include "raylib.h"
#include "structs/Brush.h"
#include "structs/Particle.h"

int A_TARGET_FPS = 60;
const int W_MAX_PARTICLES = 200'000;
const int W_MAX_OUT_OF_BOUNDS_OFFSET = 1000;

std::vector<Particle> particles;

ParticleType fire{.minInertia = {-1, -1},
                  .maxInertia = {1, 2.5},
                  .minInertiaAdd = {-0.2, -0.1},
                  .maxInertiaAdd = {0.2, 0.15},
                  .minRandMove = {-2, -0.8},
                  .maxRandMove = {2, 2},
                  .color = RED,
                  .size = 4,
                  .lifeDecay = 1.5};

ParticleType water{.minInertia = {-1, -4},
                   .maxInertia = {1, 0},
                   .minInertiaAdd = {-0.5, -2},
                   .maxInertiaAdd = {0.5, -1},
                   .minRandMove = {-0.5, 0},
                   .maxRandMove = {0.5, 0},
                   .color = BLUE,
                   .size = 4,
                   .lifeDecay = 0.5};

void ClearAllParticles(std::vector<Particle> &particles) { particles.clear(); }

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

void UpdateParticlePosition(Particle &p)
{
    p.pos.x += (p.inertia.x + RandFloat(p.type->minRandMove.x, p.type->maxRandMove.x)) * GetFrameTime() * 100;
    p.pos.y -= (p.inertia.y + RandFloat(p.type->minRandMove.y, p.type->maxRandMove.y)) * GetFrameTime() * 100;
}

void UpdateParticleState(Particle &p)
{
    p.life -= p.type->lifeDecay;
    p.color.a = p.type->colorAlphaAdd + p.life * p.type->lifeAlphaMultiplier;

    auto inertDeltaX = RandFloat(p.type->minInertiaAdd.x, p.type->maxInertiaAdd.x);
    if (p.inertia.x + inertDeltaX > p.type->minInertia.x && p.inertia.x + inertDeltaX < p.type->maxInertia.x) {
        p.inertia.x += inertDeltaX;
    }

    auto inertDeltaY = RandFloat(p.type->minInertiaAdd.y, p.type->maxInertiaAdd.y);
    if (p.inertia.y + inertDeltaY > p.type->minInertia.y && p.inertia.y + inertDeltaY < p.type->maxInertia.y) {
        p.inertia.y += inertDeltaY;
    }

    UpdateParticlePosition(p);
}

void UpdateParticles(std::vector<Particle> &particles)
{
    ClearInvisibleParticles(particles);
    for (auto &p : particles) {
        UpdateParticleState(p);
    }
}

void DrawParticles(const std::vector<Particle> &particles)
{
    for (const auto &p : particles) {
        DrawRectangle(p.pos.x, p.pos.y, p.size, p.size, p.color);
    }
}

void AddParticlesAtPos(std::vector<Particle> &particles, ParticleType *type, const Vector2 pos, const int amount,
                       const int spread)
{
    for (int i = 0; i < amount; i++) {
        Vector2 randOffset = RandPointInCircle(spread);
        particles.push_back(
            {type, {pos.x + randOffset.x, pos.y + randOffset.y}, {0, 0}, type->color, type->startLife, type->size});
    }
}

void AddParticlesAtMousePos(std::vector<Particle> &particles, ParticleType *type, const int amount, const int spread)
{
    Vector2 mousePos = GetMousePosition();
    AddParticlesAtPos(particles, type, mousePos, amount, spread);
}

void HandleKeyboardInput(std::vector<Particle> &particles, std::vector<Particle> &savedParticles, Brush &fireBrush,
                         Brush &waterBrush, Brush *&currentBrush)
{
    switch (GetKeyPressed()) {
    case KEY_W:
        currentBrush = &waterBrush;
        break;
    case KEY_F:
        currentBrush = &fireBrush;
        break;
    case KEY_Z:
        ClearAllParticles(particles);
        break;
    case KEY_C:
        currentBrush->particleType.color = RandColorRgb(0, 0, 0, 255, 255, 255);
        break;
    case KEY_S:
        savedParticles = particles;
        break;
    case KEY_L:
        particles = savedParticles;
        break;
    case KEY_MINUS:
        currentBrush->particleType.size -= 1;
        break;
    case KEY_EQUAL:
        currentBrush->particleType.size += 1;
        break;
    }
}

void DrawDebugInfo(const std::vector<Particle> &particles)
{
    DrawFPS(1200, 20);
    DrawText(std::to_string(particles.size()).c_str(), 1200, 40, 24, GRAY);
}

void AppInit()
{
    InitWindow(1280, 800, "hiasobi (camellia reference!!!)");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(A_TARGET_FPS);
    SetTraceLogLevel(LOG_DEBUG);
    SetExitKey(KEY_ESCAPE);
}

// TODO: this function is a giant mess
void StartMenu()
{
    while (!IsKeyPressed(KEY_ENTER)) {
        if (WindowShouldClose()) {
            CloseWindow();
            exit(0);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        AddParticlesAtPos(particles, &fire, {(float)GetScreenWidth() / 2 - 300, (float)GetScreenHeight() / 2 + 100}, 16,
                          32);
        AddParticlesAtPos(particles, &fire, {(float)GetScreenWidth() / 2 + 300, (float)GetScreenHeight() / 2 + 100}, 16,
                          32);
        UpdateParticles(particles);
        DrawParticles(particles);
        DrawText("hiasobi", GetScreenWidth() / 2 - 110, GetScreenHeight() / 2 - 180, 64, RED);
        DrawText(" it took me like 5 minutes\n\nto come up with the name", GetScreenWidth() / 2 - 150,
                 GetScreenHeight() / 2 - 100, 24, RED);
        DrawText("press enter to play", GetScreenWidth() / 2 - 140, GetScreenHeight() / 2 + 100, 28, GRAY);
        EndDrawing();
    }
}

int main()
{
    AppInit();
    std::vector<Particle> savedParticles;
    Brush fireBrush = {.particleType = fire, .amount = 16, .spread = 16};
    Brush waterBrush = {.particleType = water, .amount = 32, .spread = 32};
    Brush *currentBrush = &fireBrush;

    StartMenu();
    // game loop
    while (!WindowShouldClose()) {
        UpdateParticles(particles);
        HandleKeyboardInput(particles, savedParticles, fireBrush, waterBrush, currentBrush);

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_X)) {
            AddParticlesAtMousePos(particles, &currentBrush->particleType, currentBrush->amount, currentBrush->spread);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawParticles(particles);
        DrawDebugInfo(particles);
        EndDrawing();
    }

    CloseWindow();
}