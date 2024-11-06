#include <iostream>

#include "WorldManager.h"


WorldManager::WorldManager() : cellSize{ 10 }, worldSize{ 1000, 1000 } {
    std::cout << "World Created" << std::endl;
}

WorldManager::~WorldManager() {
    std::cout << "I am become Death, the destroyer of worlds" << std::endl;
}

void WorldManager::run() {
    // TODO: Implement Time.deltaTime or tickRate to show console output every n ticks
    ecs.update();
}
