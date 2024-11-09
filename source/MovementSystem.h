#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Common.h"


class MovementSystem {
public:
    void update(
        const float &fixedDeltaTime,
        const std::vector<Entity> &entities,
        std::unordered_map<int, Position> &positions,
        const std::unordered_map<int, Velocity> &velocities,
        const std::unordered_set<int> &entitiesToStop
    ) const;

private:
    uint8_t m_speed = 40;
};
