#pragma once

#include <unordered_set>

#include "Common.h"


class CollisionSystem {
public:
    std::unordered_set<int> entitiesToStop;

    void update(
        const EntityBuffer& entities,
        PositionBuffer& positions,
        ColliderBuffer& colliders
    );

private:
    static bool isColliding(
        const Position& posA,
        const Collider& colA,
        const Position& posB,
        const Collider& colB
    );
};
