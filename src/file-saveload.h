#pragma once
#include "raylib.h"
#include "structures/Brush.h"

/* exactly what it says - contains functions for saving / loading things from and to files */

void SaveBrushToFile(const Brush &brush, const char *filePath);
Brush LoadBrushFromFile(const char *filePath);