#pragma once
#include "raylib.h"

/* functions for generating various random data */

int RandInt(int min, int max);
double RandDouble(double min, double max);
Vector2 RandPointInCircle(float r);
Color RandColorRgb(int r_min, int g_min, int b_min, int r_max, int g_max, int b_max);
Color RandColorRgba(int r_min, int g_min, int b_min, int a_min, int r_max, int g_max, int b_max, int a_max);