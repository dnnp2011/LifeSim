#pragma once

#include <unordered_set>

#include "Common.h"


class CollisionSystem {
public:
    std::unordered_set<int> entitiesToStop;
    std::unordered_set<int> entitiesOutOfBounds;

    CollisionSystem() {
        entitiesToStop.reserve(ENTITY_COUNT);
        entitiesOutOfBounds.reserve(ENTITY_COUNT);
    };

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
