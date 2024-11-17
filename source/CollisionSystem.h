#pragma once

#include <Common.h>
#include <unordered_set>


class CollisionSystem
{
private:
    Threads::ThreadPool m_threadPool;
    std::mutex m_mtx;
    std::mutex m_oobMtx;

public:
    explicit CollisionSystem(const size_t numThreads):
        m_threadPool{ numThreads }
    { }

    void update(
        const EntityBuffer& entities,
        PositionBuffer& positions,
        VelocityBuffer& velocities,
        ColliderBuffer& colliders
    );

private:
    static bool isColliding(
        const Position& posA,
        const Collider& colA,
        const Position& posB,
        const Collider& colB
    );

    static bool isOutOfBounds(
        const Position& pos,
        const Collider& col,
        const Velocity& vel,
        int width,
        int height
    );
};
