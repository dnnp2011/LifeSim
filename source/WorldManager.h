#pragma once


struct WorldSize {
    unsigned int x;
    unsigned int y;
};

class WorldManager {
public:
    unsigned short int cellSize;

    WorldSize worldSize;

    WorldManager();

    ~WorldManager();

    void run();
};
