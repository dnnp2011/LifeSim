#include <iostream>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "MovementSystem.h"
#include "Common.h"


void MovementSystem::update(
    const float fixedDeltaTime,
    const EntityBuffer& entities,
    PositionBuffer& positions,
    VelocityBuffer& velocities,
    const std::unordered_set<int>& entitiesToStop,
    const std::unordered_set<int>& entitiesOutOfBounds
) {
    for (const auto& entity: entities) {
        Debounce(
            [entity, positions, velocities](const std::string& id) {
                const auto entityPosition{ positions.find(entity.id) };
                const auto entityVelocity{ velocities.find(entity.id) };

                std::cout << id << "---------------------------------" << "\n";
                std::cout << "Entity ID: " << entity.id << "\n";
                std::cout << "Position: (" << entityPosition->second.x << ", " << entityPosition->second.y << ")" << "\n";
                std::cout << "Velocity: (" << entityVelocity->second.dx << ", " << entityVelocity->second.dy << ")" << std::endl;
            },
            5000,
            "MovementSystem::update::" + std::to_string(entity.id)
        );

        m_threadPool.enqueue(
            [this, &fixedDeltaTime, &positions, &velocities, &entity]() {
                const auto entityPosition{ positions.find(entity.id) };
                const auto entityVelocity{ velocities.find(entity.id) };

                if (entityPosition != positions.end() && entityVelocity != velocities.end()) {
                    const std::lock_guard lock(m_mtx);

                    positions[entity.id].x += (entityVelocity->second.dx * fixedDeltaTime * static_cast<float>(m_speed));
                    positions[entity.id].y += (entityVelocity->second.dy * fixedDeltaTime * static_cast<float>(m_speed));
                }
            }
        );
    }
}
