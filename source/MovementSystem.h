#pragma once

#include <unordered_set>

#include "Common.h"


class MovementSystem {
public:
    void update(
        float fixedDeltaTime,
        const EntityBuffer& entities,
        PositionBuffer& positions,
        VelocityBuffer& velocities,
        const std::unordered_set<int>& entitiesToStop,
        const std::unordered_set<int>& entitiesOutOfBounds
    ) const;

private:
    uint8_t m_speed = 40;
};
