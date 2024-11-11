#pragma once

#include <unordered_set>

#include "Common.h"


class MovementSystem {
public:
    void update(
        float fixedDeltaTime,
        const EntityBuffer& entities,
        PositionBuffer& positions,
        const VelocityBuffer& velocities,
        const std::unordered_set<int>& entitiesToStop
    ) const;

private:
    uint8_t m_speed = 40;
};
