#include "raylib.h"
#include "structs/Brush.h"
#include "structs/ParticleType.h"
#include <fstream>
#include <iostream>

void SaveBrushToFile(const Brush &brush, const char *filePath)
{
    std::ofstream fs {filePath, std::ios::binary };
    if (!fs.is_open()) 
    {
        std::cerr << "File " << filePath << " can't be opened for writing.\n";
    }
    fs.write(reinterpret_cast<const char*>(&brush.particleType), sizeof(brush.particleType));
    fs.write(reinterpret_cast<const char*>(&brush.amount), sizeof(brush.amount));
    fs.write(reinterpret_cast<const char*>(&brush.spread), sizeof(brush.spread));
    fs.close();
}

Brush LoadBrushFromFile(const char *filePath)
{
    std::ifstream fs {filePath, std::ios::binary };
    if (!fs.is_open()) 
    {
        std::cerr << "File " << filePath << " can't be opened for reading.\n";
    }
    ParticleType pt;
    fs.read(reinterpret_cast<char*>(&pt), sizeof(pt));
    int amount;
    fs.read(reinterpret_cast<char*>(&amount), sizeof(amount));
    int spread;
    fs.read(reinterpret_cast<char*>(&spread), sizeof(spread));
    fs.close();
    return {pt, amount, spread};
}



