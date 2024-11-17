#include <Common.h>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "MovementSystem.h"


void MovementSystem::update(
    const float fixedDeltaTime,
    const EntityBuffer& entities,
    PositionBuffer& positions,
    VelocityBuffer& velocities
)
{
    for (const auto& entity: entities) {
#if 0
        Debounce(
            [entity, positions, velocities](const std::string& id) {
                const auto entityPosition{ positions.find(entity.id) };
                const auto entityVelocity{ velocities.find(entity.id) };

                fprintf(stdout, "%s---------------------------------\n", id.c_str());
                fprintf(stdout, "Entity ID: %d\n", entity.id);
                fprintf(stdout, "Position: (%.1f, %.1f)\n", entityPosition->second.x, entityPosition->second.y);
                fprintf(stdout, "Velocity: (%.1f, %.1f)\n", entityVelocity->second.dx, entityVelocity->second.dy);
            },
            5000,
            "MovementSystem::update::" + std::to_string(entity.id)
        );
#endif


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
