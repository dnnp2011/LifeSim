#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common.h"

class CollisionSystem {
public:
    std::unordered_set<int> entitiesToStop;

    void update(const std::vector<Entity> &entities, std::unordered_map<int, ImVec2> &positions, std::unordered_map<int, Collider> &colliders);

private:
    static bool isColliding(const ImVec2 &posA, const Collider &colA, const ImVec2 &posB, const Collider &colB);
};
