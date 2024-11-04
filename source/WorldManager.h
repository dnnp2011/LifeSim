#pragma once

#include <iostream>

#include "ECSManager.h"

struct WorldSize {
    unsigned short int x: 2;
    unsigned short int y: 2;

    WorldSize(const unsigned short int x, const unsigned short int y) : x(x), y(y) {}
};

class WorldManager {
public:
    unsigned short int cellSize;

    WorldSize worldSize;
    ECSManager ecs;

    explicit WorldManager() : cellSize{ 10 }, worldSize{ 1000, 1000 } {
        std::cout << "World Created" << std::endl;
    }

    ~WorldManager();

    void run() {
        ecs.update();
    }
};
