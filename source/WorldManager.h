#pragma once

#include <iostream>

#include "ECSManager.h"

struct WorldSize {
    unsigned int x;
    unsigned int y;
};

class WorldManager {
public:
    unsigned short int cellSize;

    WorldSize worldSize;
    ECSManager ecs;

    WorldManager() : cellSize{ 10 }, worldSize{ 1000, 1000 } {
        std::cout << "World Created" << std::endl;
    }

    ~WorldManager() {
        std::cout << "I am become Death, the destroyer of worlds" << std::endl;
    }

    void run() {
        ecs.update();
    }
};
