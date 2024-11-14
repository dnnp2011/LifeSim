#pragma once

#include <unordered_set>

#include "Common.h"


class MovementSystem {
private:
    Threads::ThreadPool m_threadPool;
    std::mutex m_mtx;
    uint8_t m_speed = 80;

public:
    explicit MovementSystem(const size_t numThreads): m_threadPool{ numThreads } { };

    void update(
        float fixedDeltaTime,
        const EntityBuffer& entities,
        PositionBuffer& positions,
        VelocityBuffer& velocities,
        const std::unordered_set<int>& entitiesToStop,
        const std::unordered_set<int>& entitiesOutOfBounds
    );
};
