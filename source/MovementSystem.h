#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common.h"

class MovementSystem {
public:
    void update(
        std::vector<Entity> &entities,
        std::unordered_map<int, ImVec2> &positions,
        std::unordered_map<int, Velocity> &velocities,
        const std::unordered_set<int> &entitiesToStop
    ) const;
};
