#pragma once

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

    WorldManager();

    ~WorldManager();

    void run();
};
