#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Common.h"


class CollisionSystem {
public:
    std::unordered_set<int> entitiesToStop;

    void update(const std::vector<Entity> &entities, std::unordered_map<int, Position> &positions, std::unordered_map<int, Collider> &colliders);

private:
    static bool isColliding(const Position &posA, const Collider &colA, const Position &posB, const Collider &colB);
};
