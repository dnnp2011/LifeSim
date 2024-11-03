//
// Created by Dalton Pierce on 10/30/24.
//

#include "WorldManager.h"

class WorldManager {
public:
    unsigned short int cellSize;
    unsigned short int worldSizeX;
    unsigned short int worldSizeY;

    WorldManager() : cellSize(10), worldSizeX(0), worldSizeY(0) {
        std::cout << "World Created\n";
    }
};
