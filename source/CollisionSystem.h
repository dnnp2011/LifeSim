#pragma once

#include <Common.h>


class Renderer;

class CollisionSystem {
private:
    Renderer* m_renderer;
    std::mutex& m_physicsBufferMutex;

    Threads::ThreadPool m_threadPool;

public:
    explicit CollisionSystem(std::mutex& physicsBufferMutex);

    explicit CollisionSystem(const CollisionSystem&) = default;

    explicit CollisionSystem(CollisionSystem&&) = default;

    void Update(EntityData& physicsBufferWrite);

private:
    [[nodiscard]]
    bool isColliding(
        const Position& posA,
        const Collider& colA,
        const Position& posB,
        const Collider& colB
    ) const;

    [[nodiscard]]
    bool isOutOfBounds(
        const Position& pos,
        const Collider& col,
        const Velocity& vel
    ) const;
};
