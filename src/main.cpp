#include <cstdlib>
#include <string>
#include <vector>

#include "rand-utils.h"
#include "particle-logic.h"
#include "raylib.h"
#include "structs/Brush.h"
#include "structs/Particle.h"

int A_TARGET_FPS = 60;
const int W_MAX_PARTICLES = 200'000;

std::vector<Particle> particles;

ParticleType fire{.minInertia = {-1.2, -1},
                  .maxInertia = {1.2, 2.5},
                  .minInertiaAdd = {-0.3, -0.2},
                  .maxInertiaAdd = {0.3, 0.35},
                  .minRandMove = {-4, -1},
                  .maxRandMove = {4, 3},
                  .color = RED,
                  .size = 4,
                  .lifeDecay = 1.8};
ParticleType water{.minInertia = {-1, -4},
                   .maxInertia = {1, 0},
                   .minInertiaAdd = {-0.5, -2},
                   .maxInertiaAdd = {0.5, -1},
                   .minRandMove = {-0.5, 0},
                   .maxRandMove = {0.5, 0},
                   .color = BLUE,
                   .size = 4,
                   .lifeDecay = 0.5};

/* This function contains the high-level logic of updating all particles. */
/* Should be called in the game loop.*/
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

/* Always emits the same amount of particles per frame. */
void AddParticles(std::vector<Particle> &particles, const Vector2 pos, ParticleType *type, const int amount,
                  const int spread)
{
    for (int i = 0; i < amount; i++) {
        Vector2 randOffset = RandPointInCircle(spread);
        particles.push_back(
            {type, {pos.x + randOffset.x, pos.y + randOffset.y}, {0, 0}, type->color, type->startLife, type->size});
    }
}

void AddParticles(std::vector<Particle> &particles, const Vector2 pos, Brush &brush)
{
    AddParticles(particles, pos, &brush.particleType, brush.amount, brush.spread);
}

/*
    Always emits the same amount of particles per second, regardless of framerate.

    If the actual framerate is equal to defaultFps, then emits the same amount
    of particles per frame as the AddParticles function would.
*/
void AddParticlesFpsLimited(std::vector<Particle> &particles, Vector2 pos, ParticleType *type, int amount,
                            int spread, int defaultFps = 60)
{
    AddParticles(particles, pos, type, amount * defaultFps * GetFrameTime(), spread);
}

void AddParticlesFpsLimited(std::vector<Particle> &particles, Vector2 pos, Brush &brush, int defaultFps = 60)
{
    AddParticlesFpsLimited(particles, pos, &brush.particleType, brush.amount, brush.spread, defaultFps);
}

/* Does things based on keys pressed. Not related to particle logic. */
void HandleKeyboardInput(std::vector<Particle> &particles, std::vector<Particle> &savedParticles, Brush &fireBrush,
                         Brush &waterBrush, Brush *&currentBrush)
{
    switch (GetKeyPressed()) {
    // testing
    case KEY_UP:
        A_TARGET_FPS += 10;
        SetTargetFPS(A_TARGET_FPS);
        break;
    case KEY_DOWN:
        A_TARGET_FPS -= 10;
        SetTargetFPS(A_TARGET_FPS);
        break;
    case KEY_W:
        currentBrush = &waterBrush;
        break;
    case KEY_F:
        currentBrush = &fireBrush;
        break;
    case KEY_Z:
        particles.clear();
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
    Brush fireMenuBrush = {fire, 16, 32};
    while (!IsKeyPressed(KEY_ENTER) && !IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (WindowShouldClose()) {
            CloseWindow();
            exit(0);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        AddParticles(particles, {(float)GetScreenWidth() / 2 - 300, (float)GetScreenHeight() / 2 + 100}, fireMenuBrush);
        AddParticles(particles, {(float)GetScreenWidth() / 2 + 300, (float)GetScreenHeight() / 2 + 100}, fireMenuBrush);
        UpdateParticles(particles);
        DrawParticles(particles);
        DrawText("hiasobi", GetScreenWidth() / 2 - 110, GetScreenHeight() / 2 - 180, 64, RED);
        DrawText(" it took me like 5 minutes\n\nto come up with the name", GetScreenWidth() / 2 - 150,
                 GetScreenHeight() / 2 - 100, 24, RED);
        DrawText("press enter to play", GetScreenWidth() / 2 - 140, GetScreenHeight() / 2 + 100, 28, GRAY);
        EndDrawing();
    }
    particles.clear();
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
            Vector2 mousePos = GetMousePosition();
            AddParticlesFpsLimited(particles, mousePos, *currentBrush);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawParticles(particles);
        DrawDebugInfo(particles);
        EndDrawing();
    }

    CloseWindow();
}