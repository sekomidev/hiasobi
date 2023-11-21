#include "raylib.h"
#include "rand.h"
#include <random>

std::random_device rd;
std::mt19937 mt(rd());

int RandInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

float RandFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);
}

Vector2 RandPointInCircle(float r)
{
    int a = random(), b = random();
    if (b < a) 
    {    
        std::swap(a, b);
    }
    return {b * r * std::cos(2 * PI * a / b) / INT32_MAX, b * r * std::sin(2 * PI * a / b) / INT32_MAX};
}

Color RandColorRgb(int r_min, int g_min, int b_min, int r_max, int g_max, int b_max)
{
    return (Color){
        (uint_fast8_t)(RandInt(r_min, r_max)),
        (uint_fast8_t)(RandInt(g_min, g_max)),
        (uint_fast8_t)(RandInt(b_min, b_max))
    };
}

Color RandColorRgba(int r_min, int g_min, int b_min, int a_min, int r_max, int g_max, int b_max, int a_max)
{
    return (Color){
        (uint_fast8_t)(RandInt(r_min, r_max)),
        (uint_fast8_t)(RandInt(g_min, g_max)),
        (uint_fast8_t)(RandInt(b_min, b_max)),
        (uint_fast8_t)(RandInt(a_min, a_max))
    };
}